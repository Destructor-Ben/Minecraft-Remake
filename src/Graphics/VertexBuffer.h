#pragma once

namespace Minecraft
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer& other) = delete;
        VertexBuffer(VertexBuffer&& other) = delete;
        VertexBuffer& operator =(const VertexBuffer& other) = delete;
        VertexBuffer& operator =(VertexBuffer&& other) = delete;

        void SetData(const vector<float>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const float* data, uint count, GLenum usage = GL_STATIC_DRAW);
        void SetDataRaw(const void* data, slong size, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        uint m_ID = 0;
    };
}
