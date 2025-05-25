#pragma once

#include "UI/UIElement.h"

namespace Minecraft
{
    // Root level UI object
    class UIState : public UIElement
    {
    public:
        bool IsInGameUI = true;

        void Init();
        virtual void OnInit() { };

        // Make sure the UI isn't active when it isn't meant to be
        // Always called even if not active
        virtual void CheckActive();
    };
}
