#pragma once

#include <string>
#include <glm/glm.hpp>

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(std::string vertexPath, std::string fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Uniforms
	void SetBool(const std::string& uniformName, bool value);
	void SetInt(const std::string& uniformName, int value);
	void SetFloat(const std::string& uniformName, float value);
	void SetMat4(const std::string& uniformName, glm::mat4 mat);
private:

	// Creating the program/shaders
	ShaderSource ReadSourceFiles(const std::string& vertexPath, const std::string& fragmentPath) const;
	unsigned int CreateProgram(const char* vertexSource, const char* fragmentSource) const;
	unsigned int CompileShader(const char* shaderSource, unsigned int shaderType) const;

	// Uniforms
	signed int GetUniformLocation(const std::string& uniformName);

private:
	unsigned int m_programID; 
};

