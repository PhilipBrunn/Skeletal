#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
	ShaderSource shaderSourceCode = ReadSourceFiles(vertexPath, fragmentPath);
	m_programID = CreateProgram(shaderSourceCode.vertexSource.c_str(), shaderSourceCode.fragmentSource.c_str());

}


Shader::~Shader()
{
	glDeleteProgram(m_programID);
}

void Shader::Bind() const
{
	glUseProgram(m_programID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

ShaderSource Shader::ReadSourceFiles(const std::string & vertexPath, const std::string & fragmentPath) const
{
	std::string vertexSource, fragmentSource;
	std::ifstream vertexFile, fragmentFile;

	// Allow files to throw exceptions
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexSource = vertexStream.str();
		fragmentSource = fragmentStream.str();

	}
	catch (...)
	{
		std::cout << "Error: Shader File Not Successfully Read" << std::endl;
	}

	return { vertexSource, fragmentSource };
}

unsigned int Shader::CreateProgram(const char * vertexSource, const char * fragmentSource) const
{
	unsigned int program = glCreateProgram();
	
	unsigned int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	int successLinking;
	constexpr int msgLength = 512;
	char msgLog[msgLength];

	glGetProgramiv(program, GL_LINK_STATUS, &successLinking);
	if (!successLinking)
	{
		glGetProgramInfoLog(program, msgLength, nullptr, msgLog);
		std::cout << "Error: Program Linking Failed" << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;

}

unsigned int Shader::CompileShader(const char* shaderSource, unsigned int shaderType) const
{
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	int successCompilation;
	constexpr int msgLength = 512;
	char msgLog[msgLength];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &successCompilation);
	if (!successCompilation)
	{
		glGetShaderInfoLog(shader, msgLength, nullptr, msgLog);
		std::cout << "Error: " << (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader Failed Compilation" << std::endl;
	}

	return shader;
}

signed int Shader::GetUniformLocation(const std::string & uniformName)
{
	int location = glGetUniformLocation(m_programID, uniformName.c_str());
	if (location == -1)
	{
		std::cout << "Error: uniform " << uniformName << " was not found" << std::endl;
	}
	return location;
}

void Shader::SetBool(const std::string & uniformName, bool value)
{
	SetInt(uniformName, value);
}

void Shader::SetInt(const std::string & uniformName, int value)
{
	glUniform1i(GetUniformLocation(uniformName), value);
}

void Shader::SetFloat(const std::string & uniformName, float value)
{
	glUniform1f(GetUniformLocation(uniformName), value);
}

void Shader::SetMat4(const std::string& uniformName, glm::mat4 mat)
{
	glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, glm::value_ptr(mat));
}
