#pragma once

#include "UI/UIElement.h"
#include "UI/UIState.h"

namespace Minecraft::UI
{
    // TODO: initialize the rest of the UI that is unimplemented
    // Menu
    inline UIState MainMenu;

    // Ingame
    inline UIState Crosshair;
    inline UIState Hotbar;
    inline UIState DebugMenu;

    // This is where all the UI states are initialized
    void Init();

    // All the update code for each UI state runs here
    void Update();
}
