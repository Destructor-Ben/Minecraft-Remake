#include "UIMainMenu.h"

#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISprite.h"
#include "World/World.h"

namespace Minecraft
{
    void UIMainMenu::OnInit()
    {
        IsInGameUI = false;

        // Repeating dirt texture
        auto backgroundTexture = Instance->Resources->RequestTexture("ui/menu/background");
        backgroundTexture->SetWrappingModes(GL_REPEAT);
        m_Background = make_shared<UISprite>();
        m_Background->DrawnSprite.SpriteTexture = backgroundTexture;
        m_Background->DrawnSprite.UVs = Rectangle(vec2i(0), Instance->ScreenSize / 30);
        m_Background->Width.Percent = 1;
        m_Background->Height.Percent = 1;
        AddElement(m_Background);

        auto logoTexture = Instance->Resources->RequestTexture("ui/menu/logo");
        m_Logo = make_shared<UISprite>();
        m_Logo->SetTexture(logoTexture, vec2(1.25));
        m_Logo->x.Percent = 0.5f;
        m_Logo->y.Pixels = -200;
        m_Logo->y.Percent = 1.0f;
        m_Logo->OriginX.Percent = 0.5f;
        m_Logo->OriginY.Percent = 0.5f;
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

        m_PlayButton->x.Percent = 0.5f;
        m_PlayButton->y.Pixels = -500;
        m_PlayButton->y.Percent = 1.0f;
        m_PlayButton->OriginX.Percent = 0.5f;
        m_PlayButton->OriginY.Percent = 0.5f;
        m_PlayButton->Width.HasMin = true;
        m_PlayButton->Width.MinPixels = 300;
        m_PlayButton->SetText("Play");
        AddElement(m_PlayButton);

        m_ExitButton = make_shared<UIButton>();
        m_ExitButton->OnMouseUp = []() { Instance->Close(); };
        m_ExitButton->x.Percent = 0.5f;
        m_ExitButton->y.Pixels = -500 - m_PlayButton->GetSize().y - 10;
        m_ExitButton->y.Percent = 1.0f;
        m_ExitButton->OriginX.Percent = 0.5f;
        m_ExitButton->OriginY.Percent = 0.5f;
        m_ExitButton->Width.HasMin = true;
        m_ExitButton->Width.MinPixels = 300;
        m_ExitButton->SetText("Exit");
        AddElement(m_ExitButton);
    }

    void UIMainMenu::CheckActive()
    {
        Active = !Instance->InGame;

        UIState::CheckActive();
    }
}
