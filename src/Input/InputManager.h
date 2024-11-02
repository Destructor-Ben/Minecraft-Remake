#pragma once

#include "Input/Key.h"
#include "Input/MouseButton.h"

namespace Minecraft
{
    class InputManager
    {
    public:
        static constexpr int32 KeyCount = (int32)Key::Count;
        static constexpr int32 MouseButtonCount = (int32)MouseButton::Count;

        vec2 GetMousePos() const { return m_MousePos; }
        vec2 GetMousePosDelta() const { return m_MousePos - m_OldMousePos; }

        float32 GetScrollWheelDelta() const { return m_ScrollDelta; }

        bool IsMouseButtonDown(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int32)button]; }
        bool IsMouseButtonUp(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int32)button]; }
        bool WasMouseButtonPressed(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int32)button] && !m_MouseButtonsPressedLastFrame[(int32)button]; }
        bool WasMouseButtonReleased(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int32)button] && m_MouseButtonsPressedLastFrame[(int32)button]; }

        bool IsKeyDown(Key key) const { return m_KeysPressedThisFrame[(int32)key]; }
        bool IsKeyUp(Key key) const { return !m_KeysPressedThisFrame[(int32)key]; }
        bool WasKeyPressed(Key key) const { return m_KeysPressedThisFrame[(int32)key] && !m_KeysPressedLastFrame[(int32)key];; }
        bool WasKeyReleased(Key key) const { return !m_KeysPressedThisFrame[(int32)key] && m_KeysPressedLastFrame[(int32)key]; }

        inline static bool IsCursorDisabled();
        inline static void SetCursorDisabled(bool disabled);
        inline static bool IsRawMouseMotionEnabled();
        inline static void SetRawMouseMotion(bool isRaw);

        void Update();
        void UpdateScroll(float32 xOffset, float32 yOffset);
        void PostUpdate();

    private:
        bool m_OldMousePosInitialized = false;
        vec2 m_MousePos = vec2();
        vec2 m_OldMousePos = vec2();
        float32 m_ScrollDelta = 0.0f;

        array<bool, KeyCount> m_KeysPressedThisFrame = { };
        array<bool, KeyCount> m_KeysPressedLastFrame = { };

        array<bool, MouseButtonCount> m_MouseButtonsPressedThisFrame = { };
        array<bool, MouseButtonCount> m_MouseButtonsPressedLastFrame = { };
    };
}
