#include "IndexBuffer.h"

namespace Minecraft
{
    IndexBuffer::IndexBuffer() : m_ID(0), m_Count(0)
    {
        glGenBuffers(1, &m_ID);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::SetData(const vector <uint>& data, GLenum usage)
    {
        SetData(data.data(), data.size(), usage);
    }

    void IndexBuffer::SetData(const uint* data, uint count, GLenum usage)
    {
        m_Count = count;
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (slong)(count * sizeof(uint)), data, usage);
        Unbind();
    }

    void IndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void IndexBuffer::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
