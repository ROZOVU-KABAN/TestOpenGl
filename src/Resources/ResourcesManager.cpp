#include "ResourcesManager.h"
#include "../Renderer/ShaderProgram.h"
#include "ResourcesManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include<sstream>
#include<fstream>
#include<iostream>
#include<vector>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourcesManager::ResourcesManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);

}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "Error: No vertex Shader\n";
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "Error: No fragment Shader\n";
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_sahderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString,fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	std::cerr << "Error: cant load shader program\n"
		<< "Vertex " << vertexString
		<< "\nFragment " << fragmentString<<std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::ShaderProgram> ResourcesManager::getShaderProgram(const std::string& shaderNAme)
{
	SahderProgramMap::const_iterator it = m_sahderPrograms.find(shaderNAme);
	if (it != m_sahderPrograms.end()) return it->second;
	std::cerr << "Error Cant find shader: shader Name" << shaderNAme << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	
	stbi_set_flip_vertically_on_load(true);

	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);
	if (!pixels)
	{
		std::cerr << "Error load textures " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName,std::make_shared<Renderer::Texture2D>(width,height,pixels,channels,GL_NEAREST,GL_CLAMP_TO_EDGE)).first->second;
	stbi_image_free(pixels);

	return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::getTextures(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end()) return it->second;
	std::cerr << "Error Cant find shader: shader Name" << textureName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourcesManager::loadTextureAtlas(const std::string textureName, 
	                                                                    const std::string texturePath,
																		std::vector<std::string> subTextures,
																		const unsigned int subTexturewidth,
																		const unsigned subTextureheight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));

	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();

		unsigned int  currentTextureOffsetX = 0;
		unsigned int  currentTextureOffsetY = textureHeight;

		for (const auto& currentSubTexturesName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY- subTextureheight)/textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + textureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);

			pTexture->addSubTexture(std::move(currentSubTexturesName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTexturewidth;

			if (currentTextureOffsetX >= subTexturewidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureheight;
			}
		}


	}

	return pTexture;
}

std::shared_ptr<Renderer::Sprite> ResourcesManager::loadSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{

	auto pTexture = getTextures(textureName);
	if (!pTexture)
	{
		std::cerr << "Error, cant find texture" << textureName <<"for sprite: "<<spriteName << std::endl;
	}


	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Error, cant find shader" << shaderName << "for sprite: " << spriteName << std::endl;
	}


	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(spriteName, 
		std::make_shared<Renderer::Sprite>(
		pTexture,
		subTextureName,
		pShader,
		glm::vec2(0.f,0.f),
		glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}


std::shared_ptr<Renderer::AnimatedSprite> ResourcesManager::loadAnimatedSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTextures(textureName);
	if (!pTexture)
	{
		std::cerr << "Error, cant find texture" << textureName << "for sprite: " << spriteName << std::endl;
	}


	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Error, cant find shader" << shaderName << "for sprite: " << spriteName << std::endl;
	}


	std::shared_ptr<Renderer::AnimatedSprite> newSprite = m_Animatedsprites.emplace(spriteName,
		std::make_shared<Renderer::AnimatedSprite>(
			pTexture,
			subTextureName,
			pShader,
			glm::vec2(0.f, 0.f),
			glm::vec2(spriteWidth, spriteHeight))).first->second;

	return newSprite;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourcesManager::getAnimatedSprite(const std::string& spriteName)
{
	auto it = m_Animatedsprites.find(spriteName);
	if (it != m_Animatedsprites.end()) return it->second;
	std::cerr << "Error Cant find sprite: sprite Name" << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::Sprite> ResourcesManager::getSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end()) return it->second;

	std::cerr << "Error Cant find sprite: sprite Name" << spriteName << std::endl;
	return nullptr;
}

std::string ResourcesManager::getFileString(const std::string& relativeFilePath) const
{

	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Error open file:\t" << relativeFilePath << std::endl;
		return std::string();
	}


	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}
