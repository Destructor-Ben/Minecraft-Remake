#pragma once

#include "../Common.h"

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const void* data, uint count, uint countPerVertex, uint sizeOfNumber = sizeof(float), bool isStatic = true);

        void Bind() const;

        static void Unbind();

    private:
        uint m_ID = 0;
        bool m_Static = true;
        bool m_DataAlreadySet = false;
    };
}
