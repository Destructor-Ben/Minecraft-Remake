#pragma once

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const vector<float>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const float* data, uint32 count, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint32 GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint32 m_ID = 0;
    };
}
