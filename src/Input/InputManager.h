#pragma once

#include "Input/Key.h"
#include "Input/MouseButton.h"

namespace Minecraft
{
    class InputManager
    {
    public:
        static constexpr int KeyCount = (int)Key::Count;
        static constexpr int MouseButtonCount = (int)MouseButton::Count;

        vec2 GetMousePos() const { return m_MousePos; }
        vec2 GetMousePosDelta() const { return m_MousePos - m_OldMousePos; }

        float GetScrollWheelDelta() const { return m_ScrollDelta; }

        bool IsMouseButtonDown(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int)button]; }
        bool IsMouseButtonUp(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int)button]; }
        bool WasMouseButtonPressed(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int)button] && !m_MouseButtonsPressedLastFrame[(int)button]; }
        bool WasMouseButtonReleased(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int)button] && m_MouseButtonsPressedLastFrame[(int)button]; }

        bool IsKeyDown(Key key) const { return m_KeysPressedThisFrame[(int)key]; }
        bool IsKeyUp(Key key) const { return !m_KeysPressedThisFrame[(int)key]; }
        bool WasKeyPressed(Key key) const { return m_KeysPressedThisFrame[(int)key] && !m_KeysPressedLastFrame[(int)key];; }
        bool WasKeyReleased(Key key) const { return !m_KeysPressedThisFrame[(int)key] && m_KeysPressedLastFrame[(int)key]; }

        static bool IsCursorDisabled();
        static void SetCursorDisabled(bool disabled);
        static bool IsRawMouseMotionEnabled();
        static void SetRawMouseMotion(bool isRaw);

        void Update();
        void UpdateScroll(float xOffset, float yOffset);
        void PostUpdate();

    private:
        bool m_OldMousePosInitialized = false;
        vec2 m_MousePos = vec2();
        vec2 m_OldMousePos = vec2();
        float m_ScrollDelta = 0.0f;

        array<bool, KeyCount> m_KeysPressedThisFrame = { };
        array<bool, KeyCount> m_KeysPressedLastFrame = { };

        array<bool, MouseButtonCount> m_MouseButtonsPressedThisFrame = { };
        array<bool, MouseButtonCount> m_MouseButtonsPressedLastFrame = { };
    };
}
