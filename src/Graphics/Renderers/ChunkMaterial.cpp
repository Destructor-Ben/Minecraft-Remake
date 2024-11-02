#include "ChunkMaterial.h"

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Minecraft
{
    void ChunkMaterial::Bind()
    {
        Material::Bind();

        Texture->BindTextureUnit(0);
        m_Shader->SetUniform("uTexture", 0);
    }
}
