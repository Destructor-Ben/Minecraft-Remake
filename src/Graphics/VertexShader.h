#pragma once

namespace Minecraft
{
    class VertexShader
    {
    public:
        explicit VertexShader(const string& source);
        ~VertexShader();

        uint GetID() const { return m_ID; }

    private:
        uint m_ID = 0;
    };
}
