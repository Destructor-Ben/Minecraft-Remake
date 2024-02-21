#include "VertexBuffer.h"

namespace Minecraft
{
	VertexBuffer::VertexBuffer() : m_ID(0), m_Static(false), m_DataAlreadySet(false)
	{
		glGenBuffers(1, &m_ID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void VertexBuffer::SetData(const void* data, uint count, uint countPerVertex, uint sizeOfNumber, bool isStatic)
	{
		if (m_Static && m_DataAlreadySet)
		{
			Log("Error: Static vertex buffer already has data");
			return;
		}

        m_Static = isStatic;
        m_DataAlreadySet = true;

		Bind();
		glBufferData(GL_ARRAY_BUFFER, count * countPerVertex * sizeOfNumber, data, m_Static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
