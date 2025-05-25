#pragma once

#include "UI/UIState.h"

namespace Minecraft
{
    class UIButton;
    class UISprite;

    class UIMainMenu : public UIState
    {
    public:
        virtual void OnInit() override;
        virtual void CheckActive() override;

    private:
        shared_ptr <UISprite> m_Background;
        shared_ptr <UISprite> m_Logo;

        shared_ptr <UIButton> m_PlayButton;
        shared_ptr <UIButton> m_ExitButton;
    };
}
