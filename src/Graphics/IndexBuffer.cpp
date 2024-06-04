#include "IndexBuffer.h"

namespace Minecraft
{
    IndexBuffer::IndexBuffer() : m_ID(0), m_Count(0)
    {
        glGenBuffers(1, &m_ID);
        Bind();
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::SetData(const vector<uint32>& data, GLenum usage)
    {
        SetData(data.data(), data.size(), usage);
    }

    void IndexBuffer::SetData(const uint32* data, uint32 count, GLenum usage)
    {
        m_Count = count;
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int64)(count * sizeof(uint32)), data, usage);
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
