#pragma once

#include "../Common.h"

#include "Shader.h"

namespace Minecraft
{
    // TODO: make this virtual so we can use inheritance to allow more material types
    // Materials exist so you can set a bunch of fields, then bind will automatically bind it to the shader
    // It means you don't need to bind the shader to set the uniforms, since the uniforms are bundled with it
    class Material
    {
    public:
        glm::mat4 Transform;

        Material(Shader& shader);

        void Bind();

    private:
        Shader& m_Shader;
    };
}
