#include "SkyMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void SkyMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uTime", Time);

        DayGradient->BindTextureUnit(0);
        m_Shader->SetUniform("uDayGradient", 0);

        NightGradient->BindTextureUnit(1);
        m_Shader->SetUniform("uNightGradient", 1);

        SunsetGradient->BindTextureUnit(2);
        m_Shader->SetUniform("uSunsetGradient", 2);
    }
}
