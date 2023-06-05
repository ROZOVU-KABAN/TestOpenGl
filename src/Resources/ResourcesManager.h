#pragma once
#include <string>
#include <memory>
#include<map>

namespace Renderer
{
	class ShaderProgram;
	class Texture2D;
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

private:

	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> SahderProgramMap;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;


	TexturesMap m_textures;
	SahderProgramMap m_sahderPrograms;

	std::string m_path;

};

