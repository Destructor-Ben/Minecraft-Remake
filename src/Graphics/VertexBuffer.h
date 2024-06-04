#pragma once

#include "Common.h"

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const vector<float32>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const float32* data, uint32 count, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint32 GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
