#include "FragmentShader.h"

#include "src/Game.h"

#include <fstream>

namespace Minecraft
{
    FragmentShader::FragmentShader(const string& source) : m_ID(0)
    {
        m_ID = glCreateShader(GL_FRAGMENT_SHADER);
        const char* cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);
    }

    FragmentShader::~FragmentShader()
    {
        glDeleteShader(m_ID);
    }

    uint32 FragmentShader::GetID() const
    {
        return m_ID;
    }
}
