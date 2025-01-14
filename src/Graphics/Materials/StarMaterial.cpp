#include "StarMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void StarMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uSkyDarkness", SkyDarkness);
        m_Shader->SetUniform("uTemperatureStrength", TemperatureStrength);
        m_Shader->SetUniform("uMaxBrightness", MaxBrightness);

        StarTexture->BindTextureUnit(0);
        m_Shader->SetUniform("uTexture", 0);

        TemperatureGradient->BindTextureUnit(1);
        m_Shader->SetUniform("uTemperatureGradient", 1);
    }
}
