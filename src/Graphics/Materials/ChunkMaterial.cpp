#include "ChunkMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void ChunkMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uTexture", ChunkTexture, 0);
        m_Shader->SetUniform("uAmbientLight", AmbientLight);
        m_Shader->SetUniform("uDirToSun", DirToSun);
        m_Shader->SetUniform("uDirToMoon", DirToMoon);
        m_Shader->SetUniform("uSunLight", SunLight);
        m_Shader->SetUniform("uMoonLight", MoonLight);
    }
}
