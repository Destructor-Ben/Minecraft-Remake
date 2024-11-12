#pragma once

namespace Minecraft
{
    class FragmentShader;
    class VertexShader;

    class Shader
    {
    public:
        Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader);
        ~Shader();

        void Bind();
        uint GetID() const { return m_ID; }

        template<typename T>
        void SetUniform(const string& name, T value);

        static void Unbind();

    private:
        // TODO: cache uniform locations
        int GetUniformLocation(const string& name);

        uint m_ID = 0;
    };

    template<>
    void Shader::SetUniform<float>(const string& name, float value);
    template<>
    void Shader::SetUniform<int>(const string& name, int value);
    template<>
    void Shader::SetUniform<uint>(const string& name, uint value);

    template<>
    void Shader::SetUniform<vec2>(const string& name, vec2 value);
    template<>
    void Shader::SetUniform<vec2i>(const string& name, vec2i value);

    template<>
    void Shader::SetUniform<vec3>(const string& name, vec3 value);
    template<>
    void Shader::SetUniform<vec3i>(const string& name, vec3i value);

    template<>
    void Shader::SetUniform<vec4>(const string& name, vec4 value);
    template<>
    void Shader::SetUniform<vec4i>(const string& name, vec4i value);

    template<>
    void Shader::SetUniform<mat2>(const string& name, mat2 value);
    template<>
    void Shader::SetUniform<mat3>(const string& name, mat3 value);
    template<>
    void Shader::SetUniform<mat4>(const string& name, mat4 value);
}
