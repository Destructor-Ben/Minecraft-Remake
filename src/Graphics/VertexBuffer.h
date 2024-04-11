#pragma once

#include "../Common.h"

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const float* data, uint32 size, GLenum usage = GL_STATIC_DRAW);

        void Bind() const;

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
