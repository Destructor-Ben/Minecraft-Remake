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
        // TODO: typing input

        bool IsKeyDown(Key key) const;
        bool IsKeyUp(Key key) const;
        bool WasKeyPressed(Key key) const;
        bool WasKeyReleased(Key key) const;

        void Update();

    private:
        glm::vec2 m_MousePos = glm::vec2();
        float m_ScrollDelta = 0.0f;

        bool* m_KeysPressedThisFrame = nullptr;
        bool* m_KeysPressedLastFrame = nullptr;

        bool* m_MouseButtonsPressedThisFrame = nullptr;
        bool* m_MouseButtonsPressedLastFrame = nullptr;
    };
}
