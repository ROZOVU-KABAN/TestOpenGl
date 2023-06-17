#pragma once
#include <string>
#include <memory>
#include<map>
#include<vector>


namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
}


class ResourcesManager
{
public:
	ResourcesManager(const std::string& executablePath);
	~ResourcesManager() = default;

	ResourcesManager(const ResourcesManager&) = delete;
	ResourcesManager& operator=(const ResourcesManager&) = delete;
	ResourcesManager& operator=(const ResourcesManager&&) = delete;
	ResourcesManager(const ResourcesManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderNAme);
	
	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
	std::shared_ptr<Renderer::Texture2D> getTextures(const std::string& textureName);

	std::shared_ptr<Renderer::Texture2D> loadTextureAtlas(const std::string textureName,
		                                                  const std::string texturePath,
													      std::vector<std::string> subTextures,
														  const unsigned int subTexturewidth,
														  const unsigned subTextureheight);


	std::shared_ptr<Renderer::Sprite> loadSprite(const std::string& spriteName,
												 const std::string& textureName,
												 const std::string& shaderName,
												 const unsigned int spriteWidth,
												 const unsigned int spriteHeight,
												 const std::string& subTextureName = "default");
	std::shared_ptr<Renderer::Sprite> getSprite(const std::string& spriteName);

private:

	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> SahderProgramMap;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;

	TexturesMap m_textures;
	SahderProgramMap m_sahderPrograms;
	SpritesMap m_sprites;

	std::string m_path;

};

