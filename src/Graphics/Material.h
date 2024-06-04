#pragma once

#include "Common.h"

namespace Minecraft
{
    class Shader;

    // Materials have a bunch of fields that will automatically bind it to the shader
    // It means you don't need to bind the shader to set the uniforms, since the uniforms are bundled with it
    class Material
    {
    public:
        mat4 Transform;

        Material(shared_ptr<Shader> shader) : m_Shader(shader) { }

        shared_ptr<Shader> GetShader() { return m_Shader; }

        virtual void Bind();

    protected:
        shared_ptr<Shader> m_Shader;
    };
}
