#pragma once

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        void SetData(const vector<float>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const float* data, uint count, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
