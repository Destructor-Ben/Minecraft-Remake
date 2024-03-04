#pragma once

#include "../Common.h"

namespace Minecraft
{
	class IndexBuffer
	{
	public:
        IndexBuffer();
		~IndexBuffer();

        void SetData(const uint* data, uint count, GLenum usage = GL_STATIC_DRAW);

		void Bind() const;
        uint GetCount() const;

        static void Unbind();

	private:
		uint m_ID = 0;
		uint m_Count = 0;
	};
}
