#pragma once

#include "Common.h"

namespace Minecraft
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void SetData(const vector<uint32>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const uint32* data, uint32 count, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint32 GetID() const { return m_ID; }
        uint32 GetCount() const { return m_Count; }

        static void Unbind();

    private:
        uint32 m_ID = 0;
        uint32 m_Count = 0;
    };
}
