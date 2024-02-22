#pragma once

#include "../Common.h"

#include "Key.h"

namespace Minecraft
{
    static bool* pressedThisFrame = nullptr;
    static bool* pressedLastFrame = nullptr;

    bool IsKeyDown(Key key);
    bool IsKeyUp(Key key);
    bool WasKeyPressed(Key key);
    bool WasKeyReleased(Key key);

    void InitializeInput();
    void ShutdownInput();
    void UpdateInput();

    static int GetGLFWKeyCode(Key key);
}
