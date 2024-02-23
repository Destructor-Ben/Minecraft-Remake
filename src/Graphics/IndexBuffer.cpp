#include "IndexBuffer.h"

namespace Minecraft
{
	IndexBuffer::IndexBuffer() : m_ID(0), m_Count(0), m_Static(false), m_DataAlreadySet(false)
	{
		glGenBuffers(1, &m_ID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void IndexBuffer::SetData(const uint* data, uint count, uint sizePerIndex, bool isStatic)
	{
		if (m_Static && m_DataAlreadySet)
		{
			Log("Error: Static index buffer already has data");
			return;
		}

        m_Count = count;
        m_Static = isStatic;
        m_DataAlreadySet = true;

		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizePerIndex, data, m_Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

    uint IndexBuffer::GetCount() const
    {
        return m_Count;
    }

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
