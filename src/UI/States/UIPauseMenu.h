#pragma once

#include "UI/UIState.h"

namespace Minecraft
{
    class UISprite;

    class UIPauseMenu : public UIState
    {
    public:
        virtual void Init() override;
        virtual void CheckActive() override;
        virtual void Update() override;

    private:
        shared_ptr<UISprite> m_TestSprite;
    };
}
