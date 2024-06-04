#pragma once

#include "Common.h"

namespace Minecraft
{
    class VertexBuffer;

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();

        void PushFloat(int32 count, bool normalized = false);
        void AddBuffer(shared_ptr<VertexBuffer> buffer);

        uint32 GetID() const { return m_ID; }
        shared_ptr<VertexBuffer> GetBuffer() const { return m_Buffer; }

        static void Unbind();

    private:
        struct VertexAttribute
        {
            int32 GLType = 0;
            int32 Count = 0;
            int32 Size = 0;
            bool Normalized = false;
        };

        uint32 m_ID = 0;
        shared_ptr<VertexBuffer> m_Buffer;

        // How large one vertex is
        uint32 m_Stride = 0;

        // The list of the attributes
        vector<VertexAttribute> m_Attributes;
    };
}
