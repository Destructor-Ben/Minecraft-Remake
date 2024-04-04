#include "VertexBuffer.h"

namespace Minecraft
{
	VertexBuffer::VertexBuffer() : m_ID(0)
	{
		glGenBuffers(1, &m_ID);
        Bind();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void VertexBuffer::SetData(const void* data, uint32 size, GLenum usage)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
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
