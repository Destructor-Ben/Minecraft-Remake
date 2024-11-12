#pragma once

namespace Minecraft
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

        void SetData(const vector<uint>& data, GLenum usage = GL_STATIC_DRAW);
        void SetData(const uint* data, uint count, GLenum usage = GL_STATIC_DRAW);
        void Bind();

        uint GetID() const { return m_ID; }
        uint GetCount() const { return m_Count; }

        static void Unbind();

    private:
        uint m_ID = 0;
        uint m_Count = 0;
    };
}
