#include "VertexArray.h"

namespace Minecraft
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);

    }

    static int GetSizeFromGLType(int type)
    {
        switch (type)
        {
            case GL_BYTE:
                return sizeof(GLbyte);
            case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
            case GL_SHORT:
                return sizeof(GLshort);
            case GL_UNSIGNED_SHORT:
                return sizeof(GLushort);
            case GL_INT:
                return sizeof(GLint);
            case GL_UNSIGNED_INT:
                return sizeof(GLuint);
            case GL_FLOAT:
                return sizeof(GLfloat);
            default:
                return 0;
        }
    }

    void VertexArray::Push(int type, int count, bool normalized)
    {
        m_Types.push_back(type);
        m_Counts.push_back(count);
        m_Normalized.push_back(normalized);
        m_Stride += GetSizeFromGLType(type) * count;
        m_Count++;
    }

    void VertexArray::AddBuffer(const VertexBuffer& buffer)
    {
        Bind();
        buffer.Bind();

        int64 offset = 0;
        for (int i = 0; i < m_Count; ++i)
        {
            int type = m_Types[i];
            int count = m_Counts[i];

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, count, type, m_Normalized[i] ? GL_TRUE : GL_FALSE, (int)m_Stride, (void*)offset);
            offset += GetSizeFromGLType(type) * count;
        }
    }
}
