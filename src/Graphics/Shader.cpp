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
			Log(str("Error: Shader linking failed:\n") + infoLog);
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

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

    // TODO: ideally this wont return a pointer
    Shader* Shader::FromFile(const str &filePath)
    {
        VertexShader vert = VertexShader::FromFile(filePath);
        FragmentShader frag = FragmentShader::FromFile(filePath);
        return new Shader(vert, frag);
    }
}
