#pragma once

namespace Minecraft
{
    class VertexBuffer;

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();

        // TODO: make more PushXX functions
        // TODO: make AddBuffer also clear the stride and attributes
        // This is so multiple buffers can be added
        void PushFloat(int count, bool normalized = false);
        void AddBuffer(shared_ptr<VertexBuffer> buffer);

        uint GetID() const { return m_ID; }
        shared_ptr<VertexBuffer> GetBuffer() const { return m_Buffer; }

        static void Unbind();

    private:
        struct VertexAttribute
        {
            int GLType = 0;
            int Count = 0;
            int Size = 0;
            bool Normalized = false;
        };

        uint m_ID = 0;
        shared_ptr<VertexBuffer> m_Buffer;

        // How large one vertex is
        uint m_Stride = 0;

        // The list of the attributes
        vector<VertexAttribute> m_Attributes;
    };
}
