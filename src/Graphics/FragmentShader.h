#pragma once

namespace Minecraft
{
    class FragmentShader
    {
    public:
        explicit FragmentShader(const string& name, const string& source);
        ~FragmentShader();

        FragmentShader(const FragmentShader& other) = delete;
        FragmentShader(FragmentShader&& other) = delete;
        FragmentShader& operator =(const FragmentShader& other) = delete;
        FragmentShader& operator =(FragmentShader&& other) = delete;

        uint GetID() const { return m_ID; }
        string GetName() const { return m_Name; }

    private:
        uint m_ID = 0;
        string m_Name = "";
    };
}
