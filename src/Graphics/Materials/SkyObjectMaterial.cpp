#include "SkyObjectMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void SkyObjectMaterial::Bind()
    {
        Material::Bind();

        ObjectTexture->BindTextureUnit(0);
        m_Shader->SetUniform("uTexture", 0);
    }
}
