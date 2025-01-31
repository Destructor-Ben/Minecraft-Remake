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
            m_Shader->SetUniform("uTexture", SunTexture, 0);
        }
        else
        {
            m_Shader->SetUniform("uSkyDarkness", SkyDarkness);
            m_Shader->SetUniform("uTexture", MoonTexture, 0);
        }
    }
}
