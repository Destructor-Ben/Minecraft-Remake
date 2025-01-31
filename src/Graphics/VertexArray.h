#pragma once

namespace Minecraft
{
    class VertexBuffer;

    class VertexArray
    {
    public:
        vector <shared_ptr<VertexBuffer>> Buffers;

        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray& other) = delete;
        VertexArray(VertexArray&& other) = delete;
        VertexArray& operator =(const VertexArray& other) = delete;
        VertexArray& operator =(VertexArray&& other) = delete;

        void Bind();

        void PushInt(int count, bool isInstanceData = false, bool convertTo01 = false);
        void PushUInt(int count, bool isInstanceData = false, bool convertTo01 = false);
        void PushFloat(int count, bool isInstanceData = false);
        void PushBool(int count, bool isInstanceData = false);
        void PushMat4(bool isInstanceData = false);

        void AddBuffer(shared_ptr<VertexBuffer> buffer);

        uint GetID() const { return m_ID; }

        static void Unbind();

    private:
        struct VertexAttribute
        {
            int GLType = 0;
            int Count = 0;
            int Size = 0;
            bool IsInstanceData = false;
            bool Normalized = false; // Whether int types are normalized to 0-1
        };

        uint m_ID = 0;

        // Offset used when multiple buffers are added
        uint m_AttributeOffset = 0;

        uint m_Stride = 0; // How large one vertex is
        vector <VertexAttribute> m_Attributes; // The list of the attributes
    };
}
