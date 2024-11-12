#pragma once

namespace Minecraft
{
    class FragmentShader
    {
    public:
        explicit FragmentShader(const string& source);
        ~FragmentShader();

        uint GetID() const { return m_ID; }

    private:
        uint m_ID = 0;
    };
}
