#include "UISprite.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft
{
    void UISprite::Update()
    {
        UIElement::Update();

        Position = DrawnSprite.Position;
        Size = vec2i((vec2)DrawnSprite.SpriteTexture->GetSize() * DrawnSprite.Scale);
    }

    void UISprite::Render()
    {
        UIElement::Render();

        Instance->UI->DrawSprite(DrawnSprite);
    }
}
