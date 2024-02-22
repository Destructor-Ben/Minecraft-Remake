#pragma once

#include "../Common.h"

#include "Key.h"

namespace Minecraft
{
    bool IsKeyDown(Key key);
    bool IsKeyUp(Key key);
    bool WasKeyPressed(Key key);
    bool WasKeyReleased(Key key);

    void InitializeInput();
    void ShutdownInput();
    void UpdateInput();
}
