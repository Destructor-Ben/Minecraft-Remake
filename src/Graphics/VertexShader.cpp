#include "VertexShader.h"
#include "Logger.h"

namespace Minecraft
{
    VertexShader::VertexShader(const string& name, const string& source)
    {
        m_Name = name;
        m_ID = glCreateShader(GL_VERTEX_SHADER);
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
            Logger::Warn(format("Failed to compile vertex shader '{}'. Message: {}", name, messageStr));
        }
    }

    VertexShader::~VertexShader()
    {
        glDeleteShader(m_ID);
    }
}
