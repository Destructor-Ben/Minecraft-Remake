#include "SkyMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void SkyMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uSkyDarkness", SkyDarkness);
        m_Shader->SetUniform("uSunsetStrength", SunsetStrength);
        m_Shader->SetUniform("uSunsetCoverage", SunsetCoverage);
        m_Shader->SetUniform("uSunsetColor", SunsetColor);
        m_Shader->SetUniform("uSunsetDirection", SunsetDirection);
        m_Shader->SetUniform("uDayGradient", DayGradient, 0);
        m_Shader->SetUniform("uNightGradient", NightGradient, 1);
    }
}
