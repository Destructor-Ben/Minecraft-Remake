#include "FragmentShader.h"

namespace Minecraft
{
	FragmentShader::FragmentShader(const str& source) : m_ID(0)
	{
        m_ID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);

        // Error checking
        GLint success;
        GLchar* infoLog = nullptr;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(m_ID, 512, nullptr, infoLog);
            Log(str("Error: Fragment shader compilation failed:\n") + infoLog);
        }
	}

	FragmentShader::~FragmentShader()
	{
        glDeleteShader(m_ID);
	}


    uint FragmentShader::GetID() const
    {
        return m_ID;
    }
}