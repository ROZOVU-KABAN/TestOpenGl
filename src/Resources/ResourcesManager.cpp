#include "ResourcesManager.h"
#include "../Renderer/ShaderProgram.h"
#include "ResourcesManager.h"



ResourcesManager::ResourcesManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);

}
