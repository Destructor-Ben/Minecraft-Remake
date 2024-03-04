#pragma once

#include "../Common.h"

#include "Key.h"
#include "MouseButton.h"
#include "../Graphics/Window.h"

namespace Minecraft
{
    // TODO: typing input
    // TODO: getting key names - possibly put it in localization
    // TODO: make key binds that can be reassigned
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        glm::vec2 GetMousePos() const { return m_MousePos; }
        glm::vec2 GetMousePosDelta() const { return m_MousePos - m_OldMousePos; }

        float GetScrollWheelDelta() const { return m_ScrollDelta; }

        bool IsMouseButtonDown(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int)button];}
        bool IsMouseButtonUp(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int)button]; }
        bool WasMouseButtonPressed(MouseButton button) const { return m_MouseButtonsPressedThisFrame[(int)button] && !m_MouseButtonsPressedLastFrame[(int)button]; }
        bool WasMouseButtonReleased(MouseButton button) const { return !m_MouseButtonsPressedThisFrame[(int)button] && m_MouseButtonsPressedLastFrame[(int)button]; }

        bool IsKeyDown(Key key) const { return m_KeysPressedThisFrame[(int)key]; }
        bool IsKeyUp(Key key) const { return !m_KeysPressedThisFrame[(int)key]; }
        bool WasKeyPressed(Key key) const { return m_KeysPressedThisFrame[(int)key] && !m_KeysPressedLastFrame[(int)key];; }
        bool WasKeyReleased(Key key) const { return !m_KeysPressedThisFrame[(int)key] && m_KeysPressedLastFrame[(int)key]; }

        static void SetCursorDisabled(bool disabled) { glfwSetInputMode(Window::Handle, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }
        static bool IsCursorDisabled() { return glfwGetInputMode(Window::Handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED; }
        static void SetRawMouseMotion(bool isRaw) { if (glfwRawMouseMotionSupported()) glfwSetInputMode(Window::Handle, GLFW_RAW_MOUSE_MOTION, isRaw ? GLFW_TRUE : GLFW_FALSE); }
        static bool IsRawMouseMotionEnabled() { return glfwGetInputMode(Window::Handle, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE; }

        void Update();
        void UpdateScroll(float xOffset, float yOffset);
        void PostUpdate();

    private:
        bool m_OldMousePosInitialized = false;
        glm::vec2 m_MousePos = glm::vec2();
        glm::vec2 m_OldMousePos = glm::vec2();
        float m_ScrollDelta = 0.0f;

        bool* m_KeysPressedThisFrame = nullptr;
        bool* m_KeysPressedLastFrame = nullptr;

        bool* m_MouseButtonsPressedThisFrame = nullptr;
        bool* m_MouseButtonsPressedLastFrame = nullptr;
    };
}
