#include "UISprite.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft
{
    void UISprite::Update()
    {
        UIElement::Update();

        // TODO: this sucks
        Size = Scale * (vec2)SpriteTexture->GetSize();
    }

    void UISprite::Render()
    {
        UIElement::Render();

        auto sprite = Sprite();
        sprite.Position = Position;
        sprite.Origin = Origin;
        sprite.Rotation = Rotation;
        sprite.Depth = Depth;
        sprite.Scale = Scale;
        sprite.UVs = UVs;
        sprite.Color = Color;
        sprite.SpriteTexture = SpriteTexture;
        Instance->UI->DrawSprite(sprite);
    }
}
