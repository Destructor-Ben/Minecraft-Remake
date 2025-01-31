#pragma once

namespace Minecraft
{
    class FragmentShader;
    class VertexShader;
    class Texture;
    class CubeMap;

    class Shader
    {
    public:
        Shader(const VertexShader& vertexShader, const FragmentShader& fragmentShader);
        ~Shader();

        Shader(const Shader& other) = delete;
        Shader(Shader&& other) = delete;
        Shader& operator =(const Shader& other) = delete;
        Shader& operator =(Shader&& other) = delete;

        void Bind();
        uint GetID() const { return m_ID; }

        void SetUniform(const string& name, float value);
        void SetUniform(const string& name, int value);
        void SetUniform(const string& name, uint value);
        void SetUniform(const string& name, bool value);

        void SetUniform(const string& name, vec2 value);
        void SetUniform(const string& name, vec2i value);

        void SetUniform(const string& name, vec3 value);
        void SetUniform(const string& name, vec3i value);

        void SetUniform(const string& name, vec4 value);
        void SetUniform(const string& name, vec4i value);

        void SetUniform(const string& name, mat2 value);
        void SetUniform(const string& name, mat3 value);
        void SetUniform(const string& name, mat4 value);

        void SetUniform(const string& name, shared_ptr<Texture> value, int slot);
        void SetUniform(const string& name, shared_ptr<CubeMap> value, int slot);

        static void Unbind();

    private:
        int GetUniformLocation(const string& name);

        unordered_map<string, int> m_UniformCache = { };

        uint m_ID = 0;
    };
}
