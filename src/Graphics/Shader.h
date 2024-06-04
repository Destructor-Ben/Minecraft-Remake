#pragma once

#include "Common.h"

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
        uint32 GetID() const { return m_ID; }

        template<typename T>
        void SetUniform(const string& name, T value);

        template<>
        void SetUniform<float32>(const string& name, float32 value);
        template<>
        void SetUniform<int32>(const string& name, int32 value);
        template<>
        void SetUniform<uint32>(const string& name, uint32 value);

        template<>
        void SetUniform<vec2>(const string& name, vec2 value);
        template<>
        void SetUniform<vec2i>(const string& name, vec2i value);

        template<>
        void SetUniform<vec3>(const string& name, vec3 value);
        template<>
        void SetUniform<vec3i>(const string& name, vec3i value);

        template<>
        void SetUniform<vec4>(const string& name, vec4 value);
        template<>
        void SetUniform<vec4i>(const string& name, vec4i value);

        template<>
        void SetUniform<mat2>(const string& name, mat2 value);
        template<>
        void SetUniform<mat3>(const string& name, mat3 value);
        template<>
        void SetUniform<mat4>(const string& name, mat4 value);

        static void Unbind();

    private:
        int32 GetUniformLocation(const string& name);

        uint32 m_ID = 0;
    };
}
