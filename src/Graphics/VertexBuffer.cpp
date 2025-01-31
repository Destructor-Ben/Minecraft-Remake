#include "VertexBuffer.h"

namespace Minecraft
{
    VertexBuffer::VertexBuffer() : m_ID(0)
    {
        glGenBuffers(1, &m_ID);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void VertexBuffer::SetData(const vector<float>& data, GLenum usage)
    {
        SetData(data.data(), data.size(), usage);
    }

    void VertexBuffer::SetData(const float* data, uint count, GLenum usage)
    {
        SetDataRaw(data, count * sizeof(float), usage);
    }

    void VertexBuffer::SetDataRaw(const void* data, slong size, GLenum usage)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        Unbind();
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
