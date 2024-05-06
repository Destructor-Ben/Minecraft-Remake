#include "Shader.h"

#include "src/Game.h"

#include <fstream>

namespace Minecraft
{
    Shader::Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader) : m_ID(0)
    {
        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader.GetID());
        glAttachShader(m_ID, fragmentShader.GetID());
        glLinkProgram(m_ID);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ID);
    }

    void Shader::SetUniform(const string& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Shader::SetUniform(const string& name, int32 value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    int Shader::GetUniformLocation(const string& name)
    {
        return glGetUniformLocation(m_ID, name.c_str());
    }
}
