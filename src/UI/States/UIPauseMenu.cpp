#include "UIPauseMenu.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Input/Input.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISprite.h"
#include "UI/Elements/UIText.h"

namespace Minecraft
{
    void UIPauseMenu::OnInit()
    {
        // TODO: Future changes
        // - Blur
        // - Fade in and out
        // - Move to a separate layer so all things that open UI can create this effect
        auto backgroundTexture = Resources::RequestTexture("pixel");
        m_BlackBackground = make_shared<UISprite>();
        m_BlackBackground->DrawnSprite.SpriteTexture = backgroundTexture;
        m_BlackBackground->DrawnSprite.SpriteColor = Color(Colors::Black.RGB, 0.35f);
        m_BlackBackground->Width.Percent = 1;
        m_BlackBackground->Height.Percent = 1;
        AddElement(m_BlackBackground);

        m_PauseMenuText = make_shared<UIText>();
        m_PauseMenuText->SetText("Game is paused");
        m_PauseMenuText->x.Percent = 0.5f;
        m_PauseMenuText->y.Percent = 1;
        m_PauseMenuText->y.Pixels = -400;
        m_PauseMenuText->OriginX.Percent = 0.5;
        m_PauseMenuText->OriginX.Percent = 0.5;
        AddElement(m_PauseMenuText);

        m_BackButton = make_shared<UIButton>();
        m_BackButton->OnMouseUp = []()
        {
            Instance->IsPaused = false;
            Instance->SetMouseHidden(true);
        };

        m_BackButton->SetText("Back to game");
        m_BackButton->x.Percent = 0.5f;
        m_BackButton->y.Pixels = -500;
        m_BackButton->y.Percent = 1.0f;
        m_BackButton->OriginX.Percent = 0.5f;
        m_BackButton->OriginY.Percent = 0.5f;
        m_BackButton->Width.HasMin = true;
        m_BackButton->Width.MinPixels = 300;
        AddElement(m_BackButton);

        m_ExitButton = make_shared<UIButton>();
        m_ExitButton->OnMouseUp = []() { Instance->ExitCurrentWorld(); };
        m_ExitButton->SetText("Exit");
        m_ExitButton->x.Percent = 0.5f;
        m_ExitButton->y.Pixels = -500 - m_BackButton->GetSize().y - 10;
        m_ExitButton->y.Percent = 1.0f;
        m_ExitButton->OriginX.Percent = 0.5f;
        m_ExitButton->OriginY.Percent = 0.5f;
        m_ExitButton->Width.HasMin = true;
        m_ExitButton->Width.MinPixels = 300;
        AddElement(m_ExitButton);
    }

    void UIPauseMenu::CheckActive()
    {
        // Pausing with escape
        if (Instance->InGame && Input::WasKeyReleased(Key::Escape))
        {
            Instance->IsPaused = !Instance->IsPaused;
            Instance->SetMouseHidden(!Instance->IsPaused);
        }

        Active = Instance->IsPaused;

        UIState::CheckActive();
    }
}
