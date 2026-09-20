#include "SunMoonMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void SunMoonMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uMinBrightness", MinBrightness);

        // TODO: don't handle the material like this :/ just change the values whenever i call render
        if (IsSun)
        {
            m_Shader->SetUniform("uSkyDarkness", 1.0f);
            m_Shader->SetUniform("uTexture", SunTexture, 0);
            m_Shader->SetUniform("uGlowSize", SunGlowSize);
            m_Shader->SetUniform("uGlowStrength", SunGlowStrength);
            m_Shader->SetUniform("uGlowColor", SunGlowColor);
        }
        else
        {
            m_Shader->SetUniform("uSkyDarkness", SkyDarkness);
            m_Shader->SetUniform("uTexture", MoonTexture, 0);
            m_Shader->SetUniform("uGlowSize", MoonGlowSize);
            m_Shader->SetUniform("uGlowStrength", MoonGlowStrength);
            m_Shader->SetUniform("uGlowColor", MoonGlowColor);
        }
    }
}
