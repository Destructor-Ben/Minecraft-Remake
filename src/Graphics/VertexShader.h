#pragma once

namespace Minecraft
{
    class VertexShader
    {
    public:
        explicit VertexShader(const string& name, const string& source);
        ~VertexShader();

        VertexShader(const VertexShader& other) = delete;
        VertexShader(VertexShader&& other) = delete;
        VertexShader& operator =(const VertexShader& other) = delete;
        VertexShader& operator =(VertexShader&& other) = delete;

        uint GetID() const { return m_ID; }
        string GetName() const { return m_Name; }

    private:
        uint m_ID = 0;
        string m_Name = "";
    };
}
