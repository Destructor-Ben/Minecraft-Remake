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

	void IndexBuffer::SetData(const uint32* data, uint32 count, GLenum usage)
	{
        m_Count = count;
		Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), data, usage);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

    uint32 IndexBuffer::GetCount() const
    {
        return m_Count;
    }

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
