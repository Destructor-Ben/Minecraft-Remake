#include "SkyMaterial.h"

#include "Graphics/CubeMap.h"
#include "Graphics/Shader.h"

namespace Minecraft
{
    void SkyMaterial::Bind()
    {
        Material::Bind();

        Texture->BindTextureUnit(0);
        m_Shader->SetUniform("uSkyBox", 0);
    }
}
