#pragma once

#include "../Common.h"

#include "Key.h"

// TODO: mouse buttons - could just implement as a key internally and have special functions for them
namespace Minecraft
{
    extern glm::vec2 MousePos;

    bool IsKeyDown(Key key);
    bool IsKeyUp(Key key);
    bool WasKeyPressed(Key key);
    bool WasKeyReleased(Key key);

    void InitializeInput();
    void ShutdownInput();
    void UpdateInput();
}
