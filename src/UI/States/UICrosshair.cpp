#include "UICrosshair.h"

#include "Game.h"
#include "ResourceManager.h"
#include "UI/Elements/UISprite.h"

namespace Minecraft
{
    void UICrosshair::Init()
    {
        UIState::Init();

        auto texture = Instance->Resources->RequestTexture("ui/crosshair");
        m_Crosshair = make_shared<UISprite>();
        m_Crosshair->SpriteTexture = texture;
        m_Crosshair->Scale = vec2(3.0f);
        m_Crosshair->Origin = texture->GetSize() / 2;

        AddElement(m_Crosshair);
    }

    void UICrosshair::CheckActive()
    {
        Active = !Instance->IsPaused;

        UIState::CheckActive();
    }

    void UICrosshair::Update()
    {
        UIState::Update();

        m_Crosshair->Position = Instance->ScreenSize / 2;
    }
}
