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
        RecalculateBounds();
    }

    void UISprite::SetScale(float scale)
    {
        RecalculateBounds();
    }

    void UISprite::RecalculateBounds()
    {
        UIElement::RecalculateBounds();

        //TODO:DrawnSprite.SetTargetRect(GetBounds(), GetOrigin());
    }

    void UISprite::OnRender()
    {
        Instance->UI->DrawSprite(DrawnSprite);
    }
}
