#pragma once

#include "UI/UIState.h"

namespace Minecraft
{
    class UIButton;
    class UISprite;
    class UIText;

    class UIPauseMenu : public UIState
    {
    public:
        virtual void OnInit() override;
        virtual void CheckActive() override;

    private:
        shared_ptr <UISprite> m_BlackBackground;
        shared_ptr <UIText> m_PauseMenuText;
        shared_ptr <UIButton> m_BackButton;
        shared_ptr <UIButton> m_ExitButton;
    };
}
