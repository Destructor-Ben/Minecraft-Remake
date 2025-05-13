#include "UIMainMenu.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Input/InputManager.h"
#include "UI/Elements/UISprite.h"
#include "World/World.h"

namespace Minecraft
{
    void UIMainMenu::Init()
    {
        UIState::Init();

        IsInGameUI = false;

        auto texture = Instance->Resources->RequestTexture("chunk");
        m_TestSprite = make_shared<UISprite>();
        m_TestSprite->DrawnSprite.SpriteTexture = texture;
        m_TestSprite->DrawnSprite.Scale = 3.0f;
        m_TestSprite->DrawnSprite.Origin = texture->GetSize() / 2;
        m_TestSprite->DrawnSprite.Position = vec2i(500, 500);

        AddElement(m_TestSprite);
    }

    void UIMainMenu::CheckActive()
    {
        Active = !Instance->InGame;

        UIState::CheckActive();
    }

    void UIMainMenu::Update()
    {
        UIState::Update();

        if (!Instance->Input->WasMouseButtonPressed(MouseButton::Left))
            return;

        // Generate world
        Instance->CurrentWorld = make_shared<World>();
        Instance->InGame = true;
        Instance->IsPaused = false;
        Instance->SetMouseHidden(true);
    }
}
