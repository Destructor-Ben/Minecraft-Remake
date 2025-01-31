#pragma once

namespace Minecraft
{
    class VertexShader
    {
    public:
        explicit VertexShader(const string& source);
        ~VertexShader();

        VertexShader(const VertexShader& other) = delete;
        VertexShader(VertexShader&& other) = delete;
        VertexShader& operator =(const VertexShader& other) = delete;
        VertexShader& operator =(VertexShader&& other) = delete;

        uint GetID() const { return m_ID; }

    private:
        uint m_ID = 0;
    };
}
