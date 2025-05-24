#include "UIPauseMenu.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISprite.h"
#include "UI/Elements/UIText.h"

namespace Minecraft
{
    void UIPauseMenu::Init()
    {
        /*
        // TODO: blur in the future
        // TODO: fade in future
        // TODO: make a separate layer
        auto backgroundTexture = Instance->Resources->RequestTexture("pixel");
        m_BlackBackground = make_shared<UISprite>();
        m_BlackBackground->DrawnSprite.SpriteTexture = backgroundTexture;
        m_BlackBackground->DrawnSprite.SpriteColor = Color(Colors::Black.RGB, 0.35f);
        m_BlackBackground->Position = vec2i(0);
        AddElement(m_BlackBackground);

        m_PauseMenuText = make_shared<UIText>();
        m_PauseMenuText->SetText("Game is paused");
        m_PauseMenuText->Origin = m_PauseMenuText->Size / 2;
        AddElement(m_PauseMenuText);

        m_BackButton = make_shared<UIButton>();
        // TODO: min and max size
        //m_BackButton->SetButtonSize(300, 30);
        m_BackButton->SetText("Back");
        m_BackButton->Origin = m_BackButton->Size / 2;
        m_BackButton->OnMouseUp = []()
        {
            Instance->IsPaused = false;
            Instance->SetMouseHidden(true);
        };
        AddElement(m_BackButton);

        m_ExitButton = make_shared<UIButton>();
        // TODO: min and max size m_ExitButton->SetButtonSize(300, 30);
        m_ExitButton->SetText("Exit");
        m_ExitButton->Origin = m_ExitButton->Size / 2;
        m_ExitButton->OnMouseUp = []()
        {
            Instance->IsPaused = true;
            Instance->InGame = false;
            Instance->CurrentWorld = nullptr;
            Instance->SetMouseHidden(false);
        };
        AddElement(m_ExitButton);

        //*/

        UIState::Init();
    }

    void UIPauseMenu::CheckActive()
    {
        // Pausing with escape
        if (Instance->InGame && Instance->Input->WasKeyReleased(Key::Escape))
        {
            Instance->IsPaused = !Instance->IsPaused;
            Instance->SetMouseHidden(!Instance->IsPaused);
        }

        Active = Instance->IsPaused;

        UIState::CheckActive();
    }

    void UIPauseMenu::Update()
    {
        // Update the background scale
        // TODO: OnResize hook?
        m_BlackBackground->DrawnSprite.Scale = Instance->ScreenSize;

        // Update locations for the UI elements
        /*/ TODO: this should be done automatically with custom units
        int halfWidth = Instance->ScreenWidth / 2;
        m_PauseMenuText->Position.x = halfWidth;
        m_BackButton->Position.x = halfWidth;
        m_ExitButton->Position.x = halfWidth;

        int yPos = Instance->ScreenHeight - 400;
        m_PauseMenuText->Position.y = yPos;
        yPos -= 150;
        m_BackButton->Position.y = yPos;
        yPos -= 75;
        m_ExitButton->Position.y = yPos;*/

        UIState::Update();
    }
}
