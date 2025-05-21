#include "SpriteMaterial.h"

#include "Graphics/Shader.h"

namespace Minecraft
{
    void SpriteMaterial::Bind()
    {
        Material::Bind();

        vec2 textureSize = DrawnSprite->SpriteTexture->GetSize();
        m_Shader->SetUniform("uTexture", DrawnSprite->SpriteTexture, 0);
        // TODO: use Color.ToVec4()
        vec4 color = vec4();
        color.r = DrawnSprite->Color.r;
        color.g = DrawnSprite->Color.g;
        color.b = DrawnSprite->Color.b;
        color.a = DrawnSprite->Opacity;
        m_Shader->SetUniform("uColor", color);
        m_Shader->SetUniform("uOrigin", DrawnSprite->Origin / textureSize);

        vec2 texCoordPos = vec2(0);
        vec2 textCoordScale = vec2(1);

        if (DrawnSprite->UVs.has_value())
        {
            texCoordPos = (vec2)DrawnSprite->UVs->GetPosition() / textureSize;
            textCoordScale = (vec2)DrawnSprite->UVs->GetSize() / textureSize;
        }

        m_Shader->SetUniform("uTexCoordPosition", texCoordPos);
        m_Shader->SetUniform("uTexCoordScale", textCoordScale);
    }
}
