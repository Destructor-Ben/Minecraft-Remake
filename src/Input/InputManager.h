#pragma once

#include "Common.h"
#include "Game.h"
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

        static bool IsCursorDisabled() { return glfwGetInputMode(Window::Handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED; }
        static void SetCursorDisabled(bool disabled) { glfwSetInputMode(Window::Handle, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }
        static bool IsRawMouseMotionEnabled() { return glfwGetInputMode(Window::Handle, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE; }
        static void SetRawMouseMotion(bool isRaw)
        {
            if (glfwRawMouseMotionSupported())
                glfwSetInputMode(Window::Handle, GLFW_RAW_MOUSE_MOTION, isRaw ? GLFW_TRUE : GLFW_FALSE);
        }

        void Update();
        void UpdateScroll(float32 xOffset, float32 yOffset);
        void PostUpdate();

    private:
        bool m_OldMousePosInitialized = false;
        vec2 m_MousePos = vec2();
        vec2 m_OldMousePos = vec2();
        float32 m_ScrollDelta = 0.0f;

        array<bool, KeyCount> m_KeysPressedThisFrame = {};
        array<bool, KeyCount> m_KeysPressedLastFrame = {};

        array<bool, MouseButtonCount> m_MouseButtonsPressedThisFrame = {};
        array<bool, MouseButtonCount> m_MouseButtonsPressedLastFrame = {};
    };
}
