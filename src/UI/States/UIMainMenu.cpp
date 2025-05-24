#include "UIMainMenu.h"

#include "BoundingBox.h"
#include "Game.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Input/InputManager.h"
#include "UI/Elements/UIButton.h"
#include "UI/Elements/UISprite.h"
#include "World/World.h"

namespace Minecraft
{
    void UIMainMenu::Init()
    {
        IsInGameUI = false;

        /* TODO: reenable the UI at some point

        auto backgroundTexture = Instance->Resources->RequestTexture("ui/menu/background");
        m_Background = make_shared<UISprite>();
        m_Background->DrawnSprite.SpriteTexture = backgroundTexture;
        AddElement(m_Background);

        auto logoTexture = Instance->Resources->RequestTexture("ui/menu/logo");
        m_Logo = make_shared<UISprite>();
        m_Logo->SetTexture(logoTexture);
        m_Logo->x.Percent = 0.5f;
        m_Logo->y.Pixels = -300;
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
        m_PlayButton->SetText("Play");
        AddElement(m_PlayButton);

        m_ExitButton = make_shared<UIButton>();
        m_ExitButton->OnMouseUp = []() { Instance->Close(); };
        m_ExitButton->x.Percent = 0.5f;
        m_ExitButton->y.Pixels = -700;
        m_ExitButton->y.Percent = 1.0f;
        m_ExitButton->OriginX.Percent = 0.5f;
        m_ExitButton->OriginY.Percent = 0.5f;
        m_ExitButton->SetText("Exit");
        AddElement(m_ExitButton);

        */

        UIState::Init();
    }

    void UIMainMenu::CheckActive()
    {
        Active = !Instance->InGame;

        UIState::CheckActive();
    }

    void UIMainMenu::Render()
    {
        auto texture = Instance->Resources->RequestTexture("ui/crosshair");
        auto sprite = Sprite();
        sprite.SpriteTexture = texture;

        // Set test values for origin
        vec2 mousePos = Instance->Input->GetMousePos() / (vec2)Instance->ScreenSize;
        vec2 scale = vec2(std::lerp(1.0f, 10.0f, mousePos.x));
        float rotation = std::lerp(0, numbers::pi * 4, mousePos.y);
        sprite.Rotation = rotation;

        // Setting the origin point
        vec2i originPoint = Instance->ScreenSize / 2;

        // Drawing some test textures
        for (int i = -1; i <= 1; ++i)
        {
            Rectangle rect = Rectangle(Instance->ScreenSize / 2, (vec2)texture->GetSize() * scale);
            rect.x -= i * 30;

            sprite.SetTargetRect(rect);
            sprite.SetOriginAsPercent(vec2(0.5f));
            sprite.Origin += originPoint - sprite.Position;
            // TODO: test AccountForOrigin
            Instance->UI->DrawSprite(sprite);
        }

        UIState::Render();
    }
}
