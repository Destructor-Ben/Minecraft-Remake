#include "UIMainMenu.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISprite.h"
#include "World/World.h"

namespace Minecraft
{
    void UIMainMenu::Init()
    {
        IsInGameUI = false;

        auto backgroundTexture = Instance->Resources->RequestTexture("ui/menu/background");
        m_Background = make_shared<UISprite>();
        m_Background->DrawnSprite.SpriteTexture = backgroundTexture;
        AddElement(m_Background);

        auto logoTexture = Instance->Resources->RequestTexture("ui/menu/logo");
        m_Logo = make_shared<UISprite>();
        m_Logo->DrawnSprite.SpriteTexture = logoTexture;
        m_Logo->Origin = logoTexture->GetSize() / 2;
        m_Logo->Position.x = Instance->ScreenWidth / 2;
        m_Logo->Position.y = Instance->ScreenHeight - 300;
        AddElement(m_Logo);

        m_PlayButton = make_shared<UIButton>();
        m_PlayButton->OnMouseUp = []()
        {
            // Generate world
            Instance->Logger->Info("Entering world...");
            Instance->CurrentWorld = make_shared<World>();
            Instance->InGame = true;
            Instance->IsPaused = false;
            Instance->SetMouseHidden(true);
        };

        m_PlayButton->Position.x = Instance->ScreenWidth / 2;
        m_PlayButton->Position.y = Instance->ScreenHeight - 500;
        m_PlayButton->SetText("Play");
        m_PlayButton->Origin = m_PlayButton->Size / 2;
        AddElement(m_PlayButton);

        m_ExitButton = make_shared<UIButton>();
        m_ExitButton->OnMouseUp = []() { Instance->Close(); };
        m_ExitButton->Position.x = Instance->ScreenWidth / 2;
        m_ExitButton->Position.y = Instance->ScreenHeight - 600;
        m_ExitButton->SetText("Exit");
        m_ExitButton->Origin = m_ExitButton->Size / 2;
        AddElement(m_ExitButton);

        UIState::Init();
    }

    void UIMainMenu::CheckActive()
    {
        Active = !Instance->InGame;

        UIState::CheckActive();
    }

    void UIMainMenu::Update()
    {

        // TODO: update the locations of the UI elements

        UIState::Update();
    }
}
