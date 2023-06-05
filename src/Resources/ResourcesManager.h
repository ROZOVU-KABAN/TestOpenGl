#pragma once
#include <string>
#include <memory>
#include<map>

namespace Renderer
{
	class ShaderProgram;
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
	void loadTexture(const std::string& textureName, const std::string& texturePath);


private:

	std::string getFileString(const std::string& relativeFilePath) const;

	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> SahderProgramMap;

	SahderProgramMap m_sahderPrograms;

	std::string m_path;

};

