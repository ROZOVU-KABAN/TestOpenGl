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

	std::shared_ptr<Renderer::ShaderProgram> loadShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::ShaderProgram> getshader(const std::string& shaderNAme);
private:
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> SahderProgreamMap;

	SahderProgreamMap m_sahderPrograms;

	std::string m_path;


};

