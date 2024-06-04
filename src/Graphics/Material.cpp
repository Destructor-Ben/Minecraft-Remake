#include "Material.h"

#include "Graphics/Shader.h"

namespace Minecraft
{
    void Material::Bind()
    {
        m_Shader->Bind();
        m_Shader->SetUniform("uTransform", Transform);
    }
}
