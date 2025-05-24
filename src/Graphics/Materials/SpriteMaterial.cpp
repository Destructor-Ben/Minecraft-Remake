#include "SpriteMaterial.h"

#include "Graphics/Shader.h"

namespace Minecraft
{
    void SpriteMaterial::Bind()
    {
        Material::Bind();

        vec2i textureSize = DrawnSprite->SpriteTexture->GetSize();
        m_Shader->SetUniform("uTexture", DrawnSprite->SpriteTexture, 0);
        m_Shader->SetUniform("uColor", DrawnSprite->SpriteColor.ToRGBA());

        vec2 texCoordPos = vec2(0);
        vec2 textCoordScale = vec2(1);

        if (DrawnSprite->UVs.has_value())
        {
            texCoordPos = (vec2)DrawnSprite->UVs->GetPosition() / (vec2)textureSize;
            textCoordScale = (vec2)DrawnSprite->UVs->GetSize() / (vec2)textureSize;
        }

        m_Shader->SetUniform("uTexCoordPosition", texCoordPos);
        m_Shader->SetUniform("uTexCoordScale", textCoordScale);
    }
}
