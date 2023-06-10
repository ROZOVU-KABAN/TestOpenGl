#pragma once 
#include <glad/glad.h>
#include<string>
#include<glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader,const std::string& fragmentShader);
		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator =(const ShaderProgram&) = delete;
		ShaderProgram& operator =(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

		~ShaderProgram();
		
		bool isCompiled() const { return m_isCompiled; }
		void use()const;
		void SetInt(const std::string& name, const GLuint value);
		void SetMatrix4(const std::string& name, const  glm::mat4& matrix);



	private:
		
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		
		
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}