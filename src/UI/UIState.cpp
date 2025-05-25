#include "UIState.h"

#include "Game.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    void UIState::Init()
    {
        Width.Percent = 1.0f;
        Height.Percent = 1.0f;
        OnInit();
        RecalculateBounds();
    }

    void UIState::CheckActive()
    {
        if (IsInGameUI)
            Active &= Instance->InGame;
        else
            Active &= !Instance->InGame;
    }
}
