#pragma once

#include "UI/UIState.h"

namespace Minecraft
{
    class UISprite;

    class UICrosshair : public UIState
    {
    public:
        virtual void OnInit() override;
        virtual void CheckActive() override;

    private:
        shared_ptr <UISprite> m_Crosshair;
    };
}
