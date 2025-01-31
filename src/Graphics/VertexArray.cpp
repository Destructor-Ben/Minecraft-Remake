#include "VertexArray.h"

#include "VertexBuffer.h"

namespace Minecraft
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_ID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::PushInt(int count, bool isInstanceData, bool convertToFloat, bool normalized)
    {
        int attributeSize = sizeof(int) * count;
        m_Stride += attributeSize;
        m_Attributes.push_back({ GL_INT, count, attributeSize, isInstanceData, convertToFloat, normalized });
    }

    void VertexArray::PushUInt(int count, bool isInstanceData, bool convertToFloat, bool normalized)
    {
        int attributeSize = sizeof(uint) * count;
        m_Stride += attributeSize;
        m_Attributes.push_back({ GL_UNSIGNED_INT, count, attributeSize, isInstanceData, convertToFloat, normalized });
    }

    void VertexArray::PushFloat(int count, bool isInstanceData)
    {
        int attributeSize = sizeof(float) * count;
        m_Stride += attributeSize;
        m_Attributes.push_back({ GL_FLOAT, count, attributeSize, isInstanceData });
    }

    void VertexArray::PushMat4(bool isInstanceData)
    {
        // Mat4s are just 4 vec4s
        for (int i = 0; i < 4; ++i)
        {
            PushFloat(4, isInstanceData);
        }
    }

    void VertexArray::AddBuffer(shared_ptr <VertexBuffer> buffer)
    {
        Bind();
        buffer->Bind();
        Buffers.push_back(buffer);

        slong offset = 0;
        for (int i = 0; i < m_Attributes.size(); i++)
        {
            auto attribute = m_Attributes.at(i);

            int attributeIndex = i + m_AttributeOffset;
            glEnableVertexAttribArray(attributeIndex);

            if (attribute.GLType == GL_FLOAT || attribute.ShouldConvertToFloat)
                glVertexAttribPointer(attributeIndex, attribute.Count, attribute.GLType, attribute.IsNormalized, (int)m_Stride, (void*)offset);
            else
                glVertexAttribIPointer(attributeIndex, attribute.Count, attribute.GLType, (int)m_Stride, (void*)offset);

            // Make instance attributes update every instance
            if (attribute.IsInstanceData)
                glVertexAttribDivisor(attributeIndex, 1);

            offset += attribute.Size;
        }

        Unbind();
        buffer->Unbind();

        // Reset state so more buffers can be added
        m_Stride = 0;
        m_AttributeOffset += m_Attributes.size();
        m_Attributes.clear();
    }
}
