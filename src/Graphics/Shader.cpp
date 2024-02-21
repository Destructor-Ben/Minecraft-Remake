#include "Shader.h"

#include <fstream>
#include <sstream>

namespace Minecraft
{
	Shader::Shader(const str& filePath) : m_ID(0), m_FilePath(filePath)
	{
		m_ID = glCreateProgram();

		// Loading vertex and fragment shaders
		std::ifstream vertStream(filePath + ".vert");
		std::stringstream vertBuffer;
		vertBuffer << vertStream.rdbuf();
		vertStream.close();
		VertexShader vert(vertBuffer.str());

		std::ifstream fragStream(filePath + ".frag");
		std::stringstream fragBuffer;
		fragBuffer << fragStream.rdbuf();
		fragStream.close();
		FragmentShader frag(fragBuffer.str());

		glAttachShader(m_ID, vert.GetID());
		glAttachShader(m_ID, frag.GetID());
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
}
