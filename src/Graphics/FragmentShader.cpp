#include "FragmentShader.h"

#include <fstream>
#include <sstream>

namespace Minecraft
{
	FragmentShader::FragmentShader(const string& source) : m_ID(0)
	{
        m_ID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);

        // Error checking
        int success;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
        if (success)
            return;

        int logLength;
        glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);

        string infoLog;
        infoLog.resize(logLength);
        glGetShaderInfoLog(m_ID, logLength, nullptr, &infoLog[0]);

        Log(infoLog);
	}

	FragmentShader::~FragmentShader()
	{
        glDeleteShader(m_ID);
	}


    uint32 FragmentShader::GetID() const
    {
        return m_ID;
    }

    FragmentShader FragmentShader::FromFile(const string &filePath) {
        std::ifstream stream(filePath + ".frag");
        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        return FragmentShader(buffer.str());
    }
}
