#pragma once

#include "../Common.h"

#include <vector>

#include "VertexBuffer.h"

namespace Minecraft
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
\
        void Push(int type, int count);
        void AddBuffer(const VertexBuffer& buffer);

        static void Unbind();

    private:
        uint m_ID = 0;

        uint m_Count = 0;
        uint m_Stride = 0;
        std::vector<int> m_Types;
        std::vector<int> m_Counts;
    };
}
