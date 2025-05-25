#include "UISprite.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft
{
    void UISprite::SetTexture(shared_ptr <Texture> texture, vec2 scale)
    {
        vec2i size = texture->GetSize();
        Width.Pixels = size.x * scale.x;
        Height.Pixels = size.y * scale.y;
        DrawnSprite.SpriteTexture = texture;
        RecalculateBounds();
    }

    void UISprite::RecalculateBounds()
    {
        UIElement::RecalculateBounds();

        DrawnSprite.Position = GetPosition();
        DrawnSprite.Size = GetSize();
        DrawnSprite.Origin = GetOrigin();
    }

    void UISprite::OnRender()
    {
        Instance->UI->DrawSprite(DrawnSprite);
    }
}
