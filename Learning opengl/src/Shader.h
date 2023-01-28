#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_vertexShaderFilePath;
	std::string m_fragmentShaderFilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string ParseShader(const std::string& filepath);
public:
	Shader(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set uniforms
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

};