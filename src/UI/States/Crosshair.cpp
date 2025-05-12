#include "UI/UI.h"

#include "Game.h"
#include "ResourceManager.h"
#include "UI/Elements/UISprite.h"

namespace Minecraft::UI
{
    shared_ptr <UISprite> CrosshairSprite;

    void InitCrosshair()
    {
        Crosshair.Register();

        auto texture = Instance->Resources->RequestTexture("ui/crosshair");
        CrosshairSprite = make_shared<UISprite>();
        CrosshairSprite->DrawnSprite.SpriteTexture = texture;
        CrosshairSprite->DrawnSprite.Scale = 3.0f;
        CrosshairSprite->DrawnSprite.Origin = texture->GetSize() / 2;

        Crosshair.AddElement(CrosshairSprite);
    }

    void UpdateCrosshair()
    {
        CrosshairSprite->DrawnSprite.Position = Instance->ScreenSize / 2;
    }
}
