#pragma once

#include "UI/UIState.h"
#include "UI/States/UICrosshair.h"
#include "UI/States/UIDebug.h"
#include "UI/States/UIMainMenu.h"
#include "UI/States/UIPauseMenu.h"

namespace Minecraft::UI
{
    // TODO: move this registry boilerplate to a macro
    inline int UIStateCount = 0;
    inline vector<shared_ptr<UIState>> UIStateList = { };

    template<typename T, typename... Params>
    inline UIState* CreateUI(Params... params)
    {
        auto ui = make_shared<T>(params...);
        UIStateList.push_back(ui);
        UIStateCount++;
        ui->Init();
        return ui.get();
    }
    // END TODO

    inline constexpr int SpriteScale = 3; // Since sprites are small

    // TODO: implement these 3 properly
    inline bool ShowHud = true;
    inline bool IsInLightMode = false;
    inline float UIScale = 1.0f;

    // Menu
    inline UIState* MainMenu;

    // Ingame
    inline UIState* Crosshair;
    inline UIState* PauseMenu;
    inline UIState* DebugMenu;

    // This is where all the UI states are initialized
    inline void Init()
    {
        MainMenu = CreateUI<UIMainMenu>();

        Crosshair = CreateUI<UICrosshair>();
        PauseMenu = CreateUI<UIPauseMenu>();
        DebugMenu = CreateUI<UIDebug>();
    }
}
