#include "VertexShader.h"

#include "src/Game.h"

#include <fstream>

namespace Minecraft
{
    VertexShader::VertexShader(const string& source) : m_ID(0)
    {
        m_ID = glCreateShader(GL_VERTEX_SHADER);
        const char* cStringSource = source.c_str();
        glShaderSource(m_ID, 1, &cStringSource, nullptr);
        glCompileShader(m_ID);
    }

    VertexShader::~VertexShader()
    {
        glDeleteShader(m_ID);
    }

    uint32 VertexShader::GetID() const
    {
        return m_ID;
    }
}
