#include "ChunkMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void ChunkMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uTexture", ChunkTexture, 0);
    }
}
