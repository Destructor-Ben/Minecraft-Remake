#include "VertexArray.h"

#include "VertexBuffer.h"

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

    void VertexArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::PushFloat(int32 count, bool normalized)
    {
        int32 attributeSize = sizeof(float) * count;
        m_Stride += attributeSize;
        m_Attributes.push_back({ GL_FLOAT, count, attributeSize, normalized });
    }

    void VertexArray::AddBuffer(shared_ptr<VertexBuffer> buffer)
    {
        Bind();
        buffer->Bind();
        m_Buffer = buffer;

        int64 offset = 0;
        for (int i = 0; i < m_Attributes.size(); i++)
        {
            auto attribute = m_Attributes.at(i);

            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attribute.Count, attribute.GLType, attribute.Normalized, (int32)m_Stride, (void*)offset);
            offset += attribute.Size;
        }
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }
}
