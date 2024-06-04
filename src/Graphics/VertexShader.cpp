#include "VertexShader.h"

namespace Minecraft
{
    VertexShader::VertexShader(const string& source)
    {
        m_ID = glCreateShader(GL_VERTEX_SHADER);
        cstring cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);
    }

    VertexShader::~VertexShader()
    {
        glDeleteShader(m_ID);
    }
}
