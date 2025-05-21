#include "UISprite.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft
{
    void UISprite::SetTexture(shared_ptr <Texture> texture)
    {
        DrawnSprite.SpriteTexture = texture;
        Size = texture->GetSize();
    }

    void UISprite::Update()
    {
        UIElement::Update();

        DrawnSprite.Position = Position;
        DrawnSprite.Origin = Origin;
    }

    void UISprite::Render()
    {
        UIElement::Render();

        Instance->UI->DrawSprite(DrawnSprite);
    }
}
