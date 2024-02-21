#pragma once

#include "../Common.h"

namespace Minecraft
{
	class IndexBuffer
	{
	public:
        IndexBuffer();
		~IndexBuffer();

        void SetData(const uint* data, uint count, uint sizePerIndex = sizeof(uint), bool isStatic = true);

		void Bind() const;
        uint GetCount() const;

        static void Unbind();

	private:
		uint m_ID = 0;
		uint m_Count = 0;
		bool m_Static = true;
		bool m_DataAlreadySet = false;
	};
}
