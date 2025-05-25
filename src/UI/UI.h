#pragma once

#include "UI/UIState.h"
#include "UI/States/UICrosshair.h"
#include "UI/States/UIMainMenu.h"
#include "UI/States/UIPauseMenu.h"

namespace Minecraft::UI
{
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

    inline constexpr int SpriteScale = 3; // Since sprites are small
    inline bool IsInLightMode = false;

    // Menu
    inline UIState* MainMenu;

    // Ingame
    inline UIState* PauseMenu;
    inline UIState* Crosshair;

    // This is where all the UI states are initialized
    inline void Init()
    {
        MainMenu = CreateUI<UIMainMenu>();

        PauseMenu = CreateUI<UIPauseMenu>();
        Crosshair = CreateUI<UICrosshair>();
    }
}
