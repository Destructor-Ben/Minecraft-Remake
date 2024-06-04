#include "FragmentShader.h"

namespace Minecraft
{
    FragmentShader::FragmentShader(const string& source)
    {
        m_ID = glCreateShader(GL_FRAGMENT_SHADER);
        cstring cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);
    }

    FragmentShader::~FragmentShader()
    {
        glDeleteShader(m_ID);
    }
}
