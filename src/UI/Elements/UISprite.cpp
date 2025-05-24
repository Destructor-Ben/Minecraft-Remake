#include "UISprite.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft
{
    void UISprite::SetTexture(shared_ptr <Texture> texture)
    {
        vec2i size = texture->GetSize();
        Width.Pixels = size.x;
        Height.Pixels = size.y;
        DrawnSprite.SpriteTexture = texture;
        CalculateBounds();
    }

    void UISprite::SetScale(float scale)
    {
        CalculateBounds();
    }

    void UISprite::CalculateBounds()
    {
        UIElement::CalculateBounds();

        DrawnSprite.SetTargetRect(GetBounds(), GetOrigin());
    }

    void UISprite::Render()
    {
        UIElement::Render();

        Instance->UI->DrawSprite(DrawnSprite);
    }
}
