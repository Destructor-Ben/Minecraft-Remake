#include "UIPauseMenu.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Input/InputManager.h"
#include "UI/Elements/UISprite.h"

namespace Minecraft
{
    void UIPauseMenu::Init()
    {
        UIState::Init();

        auto texture = Instance->Resources->RequestTexture("sky/sun");
        m_TestSprite = make_shared<UISprite>();
        m_TestSprite->SpriteTexture = texture;
        m_TestSprite->Scale = vec2(3.0f);
        m_TestSprite->Origin = texture->GetSize() / 2;
        m_TestSprite->Position = vec2i(200, 200);

        AddElement(m_TestSprite);
    }

    void UIPauseMenu::CheckActive()
    {
        Active = Instance->IsPaused;

        UIState::CheckActive();
    }

    void UIPauseMenu::Update()
    {
        UIState::Update();

        if (!Instance->Input->WasMouseButtonPressed(MouseButton::Left))
            return;

        // Exit world
        Instance->IsPaused = true;
        Instance->InGame = false;
        Instance->CurrentWorld = nullptr;
        Instance->SetMouseHidden(false);
    }
}
