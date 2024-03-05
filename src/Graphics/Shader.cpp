#include "Shader.h"

#include <fstream>
#include <sstream>

namespace Minecraft
{
	Shader::Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader) : m_ID(0)
	{
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShader.GetID());
		glAttachShader(m_ID, fragmentShader.GetID());
		glLinkProgram(m_ID);

		// Error checking
		GLint success;
		GLchar* infoLog = nullptr;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
			Log(string("Error: Shader linking failed:\n") + infoLog);
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ID);
	}

    void Shader::SetUniform(const string &name, const glm::mat4& value) {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

    Shader Shader::FromFile(const string &filePath)
    {
        VertexShader vert = VertexShader::FromFile(filePath);
        FragmentShader frag = FragmentShader::FromFile(filePath);
        return Shader(vert, frag);
    }

    int Shader::GetUniformLocation(const string& name) {
        return glGetUniformLocation(m_ID, name.c_str());
    }
}
