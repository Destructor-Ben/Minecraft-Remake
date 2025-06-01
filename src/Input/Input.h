#pragma once

#include "Input/Key.h"
#include "Input/MouseButton.h"

namespace Minecraft::Input
{
    vec2 GetMousePos();
    vec2 GetMousePosDelta();
    float GetScrollWheelDelta();

    bool IsMouseButtonDown(MouseButton button);
    bool IsMouseButtonUp(MouseButton button);
    bool WasMouseButtonPressed(MouseButton button);
    bool WasMouseButtonReleased(MouseButton button);

    bool IsKeyDown(Key key);
    bool IsKeyUp(Key key);
    bool WasKeyPressed(Key key);
    bool WasKeyReleased(Key key);

    bool IsCursorDisabled();
    void SetCursorDisabled(bool disabled);
    bool IsRawMouseMotionEnabled();
    void SetRawMouseMotion(bool isRaw);

    void Update();
    void UpdateScroll(float xOffset, float yOffset);
    void PostUpdate();
}
