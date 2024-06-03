#pragma once

#include "../Common.h"

namespace Minecraft
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void SetData(const uint32* data, uint32 count, GLenum usage = GL_STATIC_DRAW);

        void Bind() const;
        uint32 GetCount() const;

        static void Unbind();

    private:
        uint32 m_ID = 0;
        uint32 m_Count = 0;
    };
}
