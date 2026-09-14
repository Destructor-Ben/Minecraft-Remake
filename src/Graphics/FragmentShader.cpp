#include "FragmentShader.h"
#include "Logger.h"

namespace Minecraft
{
    FragmentShader::FragmentShader(const string& name, const string& source)
    {
        m_Name = name;
        m_ID = glCreateShader(GL_FRAGMENT_SHADER);
        cstring cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);

        GLint compileSucceeded;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &compileSucceeded);
        if (compileSucceeded != GL_TRUE)
        {
            int bufferSize = 1024;
            GLsizei logLength = 0;
            GLchar message[bufferSize];
            glGetShaderInfoLog(m_ID, bufferSize, &logLength, message);
            string messageStr = string(message);
            Logger::Warn(format("Failed to compile fragment shader '{}'. Message: {}", name, messageStr));
        }
    }

    FragmentShader::~FragmentShader()
    {
        glDeleteShader(m_ID);
    }
}
