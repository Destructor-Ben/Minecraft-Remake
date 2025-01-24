#include "DebugMaterial.h"

#include "Graphics/Shader.h"

namespace Minecraft
{
    void DebugMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uColor", Color);
        m_Shader->SetUniform("uDrawCircle", DrawCircle);
    }
}
