#include "StarMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void StarMaterial::Bind()
    {
        Material::Bind();

        StarTexture->BindTextureUnit(0);
        m_Shader->SetUniform("uTexture", 0);
    }
}
