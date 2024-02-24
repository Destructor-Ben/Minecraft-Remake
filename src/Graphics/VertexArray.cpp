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
        switch (type) {
            case GL_BYTE:
                return sizeof(char);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
            case GL_SHORT:
                return sizeof(short);
            case GL_UNSIGNED_SHORT:
                return sizeof(unsigned short);
            case GL_INT:
                return sizeof(int);
            case GL_UNSIGNED_INT:
                return sizeof(uint);
            case GL_FLOAT:
                return sizeof(float);
            default:
                return 0;
        }
    }

    void VertexArray::Push(int type, int count) {
        m_Types.push_back(type);
        m_Counts.push_back(count);
        m_Stride += GetSizeFromGLType(type) * count;
        m_Count++;
    }

    void VertexArray::AddBuffer(const VertexBuffer& buffer) {
        Bind();
        buffer.Bind();

        int offset = 0;
        for (int i = 0; i < m_Count; ++i) {
            int type = m_Types[i];
            int count = m_Counts[i];

            // TODO: allow normalizing the data
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, count, type, GL_FALSE, (int)m_Stride, (void*)offset);
            offset += GetSizeFromGLType(type) * count;
        }
    }
}
