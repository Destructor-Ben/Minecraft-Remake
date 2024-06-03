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

        void Push(int type, int count, bool normalized = false);
        void AddBuffer(const VertexBuffer& buffer);

        static void Unbind();

    private:
        uint32 m_ID = 0;

        uint32 m_Count = 0;
        uint32 m_Stride = 0;
        vector<int> m_Types;
        vector<int> m_Counts;
        vector<bool> m_Normalized;
    };
}
