#pragma once

#include "../Common.h"

#include "Key.h"
#include "MouseButton.h"

namespace Minecraft
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        glm::vec2 GetMousePos() const;
        float GetScrollWheelDelta() const;

        bool IsMouseButtonDown(MouseButton button) const;
        bool IsMouseButtonUp(MouseButton button) const;
        bool WasMouseButtonPressed(MouseButton button) const;
        bool WasMouseButtonReleased(MouseButton button) const;
        // TODO: raw mouse motion
        // TODO: hiding cursor

        bool IsKeyDown(Key key) const;
        bool IsKeyUp(Key key) const;
        bool WasKeyPressed(Key key) const;
        bool WasKeyReleased(Key key) const;

        void Update();

    private:
        glm::vec2 m_MousePos = glm::vec2();

        bool* m_PressedThisFrame = nullptr;
        bool* m_PressedLastFrame = nullptr;
    };
}
