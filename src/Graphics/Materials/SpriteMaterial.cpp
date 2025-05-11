#include "SpriteMaterial.h"

#include "Graphics/Shader.h"

namespace Minecraft
{
    void SpriteMaterial::Bind()
    {
        Material::Bind();

        m_Shader->SetUniform("uTexture", DrawnSprite->SpriteTexture, 0);
        m_Shader->SetUniform("uColor", DrawnSprite->Color);
        m_Shader->SetUniform("uOrigin", DrawnSprite->Origin / (vec2)DrawnSprite->SpriteTexture->GetSize());
        m_Shader->SetUniform("uTexCoordPosition", DrawnSprite->UVPosition);
        m_Shader->SetUniform("uTexCoordScale", DrawnSprite->UVScale);
    }
}
