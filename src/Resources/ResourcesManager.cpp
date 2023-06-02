#include "ResourcesManager.h"
#include "../Renderer/ShaderProgram.h"
#include "ResourcesManager.h"

#include<sstream>
#include<fstream>
#include<iostream>



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
