#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath):
	m_FilePath(filepath), m_RenderID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RenderID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RenderID));
}
void Shader::Bind() const
{
	GLCall(glUseProgram(m_RenderID));
}
void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}
void Shader::setUniform1i(const std::string& name, int v)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform1i(location, v));
}
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	GLCall(glUniform4f(location, v0,v1,v2,v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	int location = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}


unsigned int Shader::GetUniformLocation(const std::string& name)
{
	const auto it = m_UniformLocationCache.find(name);
	if (it != m_UniformLocationCache.end())
		return it->second;
	else
	{
		GLCall(int location = glGetUniformLocation(m_RenderID, name.c_str()));
		//ASSERT(location != -1);
		m_UniformLocationCache[name] = location;
		return location;
	}
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	//TODO : Error handling
	return id;
}
unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX =0, FRAGMENT =1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << '\n';
		}
	}
	return {ss[0].str(), ss[1].str()};
}