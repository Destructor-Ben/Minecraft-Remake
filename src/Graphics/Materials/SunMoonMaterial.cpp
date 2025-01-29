#include "SunMoonMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void SunMoonMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uMinBrightness", MinBrightness);

        if (IsSun)
        {
            m_Shader->SetUniform("uSkyDarkness", 1.0f);

            SunTexture->BindTextureUnit(0);
            m_Shader->SetUniform("uTexture", 0);
        }
        else
        {
            m_Shader->SetUniform("uSkyDarkness", SkyDarkness);

            MoonTexture->BindTextureUnit(0);
            m_Shader->SetUniform("uTexture", 0);
        }
    }
}
