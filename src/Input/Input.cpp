#include "Input.h"

#include "Game.h"

// TODO: typing input
// TODO: getting key names - possibly put it in localization
// TODO: make key binds that can be reassigned
// TODO: x axis scrolling with mouse wheel delta
namespace Minecraft::Input
{
    static constexpr int KeyCount = (int)Key::Count;
    static constexpr int MouseButtonCount = (int)MouseButton::Count;

    bool OldMousePosInitialized = false;
    vec2 MousePos = vec2();
    vec2 OldMousePos = vec2();
    float ScrollDelta = 0.0f;

    array<bool, KeyCount> KeysPressedThisFrame = { };
    array<bool, KeyCount> KeysPressedLastFrame = { };

    array<bool, MouseButtonCount> MouseButtonsPressedThisFrame = { };
    array<bool, MouseButtonCount> MouseButtonsPressedLastFrame = { };

    static int GetGLFWKeyCode(Key key)
    {
        switch (key)
        {
            case Key::Zero: // Numbers
                return GLFW_KEY_0;
            case Key::One:
                return GLFW_KEY_1;
            case Key::Two:
                return GLFW_KEY_2;
            case Key::Three:
                return GLFW_KEY_3;
            case Key::Four:
                return GLFW_KEY_4;
            case Key::Five:
                return GLFW_KEY_5;
            case Key::Six:
                return GLFW_KEY_6;
            case Key::Seven:
                return GLFW_KEY_7;
            case Key::Eight:
                return GLFW_KEY_8;
            case Key::Nine:
                return GLFW_KEY_9;

            case Key::A: // Letters
                return GLFW_KEY_A;
            case Key::B:
                return GLFW_KEY_B;
            case Key::C:
                return GLFW_KEY_C;
            case Key::D:
                return GLFW_KEY_D;
            case Key::E:
                return GLFW_KEY_E;
            case Key::F:
                return GLFW_KEY_F;
            case Key::G:
                return GLFW_KEY_G;
            case Key::H:
                return GLFW_KEY_H;
            case Key::I:
                return GLFW_KEY_I;
            case Key::J:
                return GLFW_KEY_J;
            case Key::K:
                return GLFW_KEY_K;
            case Key::L:
                return GLFW_KEY_L;
            case Key::M:
                return GLFW_KEY_M;
            case Key::N:
                return GLFW_KEY_N;
            case Key::O:
                return GLFW_KEY_O;
            case Key::P:
                return GLFW_KEY_P;
            case Key::Q:
                return GLFW_KEY_Q;
            case Key::R:
                return GLFW_KEY_R;
            case Key::S:
                return GLFW_KEY_S;
            case Key::T:
                return GLFW_KEY_T;
            case Key::U:
                return GLFW_KEY_U;
            case Key::V:
                return GLFW_KEY_V;
            case Key::W:
                return GLFW_KEY_W;
            case Key::X:
                return GLFW_KEY_X;
            case Key::Y:
                return GLFW_KEY_Y;
            case Key::Z:
                return GLFW_KEY_Z;

            case Key::Grave: // Punctuation
                return GLFW_KEY_GRAVE_ACCENT;
            case Key::Minus:
                return GLFW_KEY_MINUS;
            case Key::Equals:
                return GLFW_KEY_EQUAL;
            case Key::LeftBracket:
                return GLFW_KEY_LEFT_BRACKET;
            case Key::RightBracket:
                return GLFW_KEY_RIGHT_BRACKET;
            case Key::BackSlash:
                return GLFW_KEY_BACKSLASH;
            case Key::SemiColon:
                return GLFW_KEY_SEMICOLON;
            case Key::Apostrophe:
                return GLFW_KEY_APOSTROPHE;
            case Key::Comma:
                return GLFW_KEY_COMMA;
            case Key::FullStop:
                return GLFW_KEY_PERIOD;
            case Key::ForwardSlash:
                return GLFW_KEY_SLASH;

            case Key::Escape: // Function keys
                return GLFW_KEY_ESCAPE;
            case Key::Enter:
                return GLFW_KEY_ENTER;
            case Key::Space:
                return GLFW_KEY_SPACE;
            case Key::Tab:
                return GLFW_KEY_TAB;
            case Key::Backspace:
                return GLFW_KEY_BACKSPACE;
            case Key::UpArrow:
                return GLFW_KEY_UP;
            case Key::DownArrow:
                return GLFW_KEY_DOWN;
            case Key::LeftArrow:
                return GLFW_KEY_LEFT;
            case Key::RightArrow:
                return GLFW_KEY_RIGHT;
            case Key::PageUp:
                return GLFW_KEY_PAGE_UP;
            case Key::PageDown:
                return GLFW_KEY_PAGE_DOWN;
            case Key::Home:
                return GLFW_KEY_HOME;
            case Key::End:
                return GLFW_KEY_END;
            case Key::Insert:
                return GLFW_KEY_INSERT;
            case Key::Delete:
                return GLFW_KEY_DELETE;
            case Key::CapsLock:
                return GLFW_KEY_CAPS_LOCK;
            case Key::PrintScreen:
                return GLFW_KEY_PRINT_SCREEN;
            case Key::LeftShift:
                return GLFW_KEY_LEFT_SHIFT;
            case Key::LeftControl:
                return GLFW_KEY_LEFT_CONTROL;
            case Key::LeftAlt:
                return GLFW_KEY_LEFT_ALT;
            case Key::RightShift:
                return GLFW_KEY_RIGHT_SHIFT;
            case Key::RightControl:
                return GLFW_KEY_RIGHT_CONTROL;
            case Key::RightAlt:
                return GLFW_KEY_RIGHT_ALT;

            case Key::F1: // F keys
                return GLFW_KEY_F1;
            case Key::F2:
                return GLFW_KEY_F2;
            case Key::F3:
                return GLFW_KEY_F3;
            case Key::F4:
                return GLFW_KEY_F4;
            case Key::F5:
                return GLFW_KEY_F5;
            case Key::F6:
                return GLFW_KEY_F6;
            case Key::F7:
                return GLFW_KEY_F7;
            case Key::F8:
                return GLFW_KEY_F8;
            case Key::F9:
                return GLFW_KEY_F9;
            case Key::F10:
                return GLFW_KEY_F10;
            case Key::F11:
                return GLFW_KEY_F11;
            case Key::F12:
                return GLFW_KEY_F12;
            case Key::F13:
                return GLFW_KEY_F13;
            case Key::F14:
                return GLFW_KEY_F14;
            case Key::F15:
                return GLFW_KEY_F15;
            case Key::F16:
                return GLFW_KEY_F16;
            case Key::F17:
                return GLFW_KEY_F17;
            case Key::F18:
                return GLFW_KEY_F18;
            case Key::F19:
                return GLFW_KEY_F19;
            case Key::F20:
                return GLFW_KEY_F20;
            case Key::F21:
                return GLFW_KEY_F21;
            case Key::F22:
                return GLFW_KEY_F22;
            case Key::F23:
                return GLFW_KEY_F23;
            case Key::F24:
                return GLFW_KEY_F24;
            case Key::F25:
                return GLFW_KEY_F25;

            case Key::KeyPad0: // Keypad
                return GLFW_KEY_KP_0;
            case Key::KeyPad1:
                return GLFW_KEY_KP_1;
            case Key::KeyPad2:
                return GLFW_KEY_KP_2;
            case Key::KeyPad3:
                return GLFW_KEY_KP_3;
            case Key::KeyPad4:
                return GLFW_KEY_KP_4;
            case Key::KeyPad5:
                return GLFW_KEY_KP_5;
            case Key::KeyPad6:
                return GLFW_KEY_KP_6;
            case Key::KeyPad7:
                return GLFW_KEY_KP_7;
            case Key::KeyPad8:
                return GLFW_KEY_KP_8;
            case Key::KeyPad9:
                return GLFW_KEY_KP_9;
            case Key::KeyPadAdd:
                return GLFW_KEY_KP_ADD;
            case Key::KeyPadSubtract:
                return GLFW_KEY_KP_SUBTRACT;
            case Key::KeyPadMultiply:
                return GLFW_KEY_KP_MULTIPLY;
            case Key::KeyPadDivide:
                return GLFW_KEY_KP_DIVIDE;
            case Key::KeyPadDecimal:
                return GLFW_KEY_KP_DECIMAL;
            case Key::KeyPadEquals:
                return GLFW_KEY_KP_EQUAL;
            case Key::KeyPadEnter:
                return GLFW_KEY_KP_ENTER;

            default: // Unknown key
                return -1;
        }
    }

    static int GetGLFWMouseButtonCode(MouseButton button)
    {
        switch (button)
        {
            case MouseButton::Left:
                return GLFW_MOUSE_BUTTON_LEFT;
            case MouseButton::Right:
                return GLFW_MOUSE_BUTTON_RIGHT;
            case MouseButton::Middle:
                return GLFW_MOUSE_BUTTON_MIDDLE;
            case MouseButton::X1:
                return GLFW_MOUSE_BUTTON_4;
            case MouseButton::X2:
                return GLFW_MOUSE_BUTTON_5;
            default:
                return -1;
        }
    }

    vec2 GetMousePos() { return MousePos; }
    vec2 GetMousePosDelta() { return MousePos - OldMousePos; }

    float GetScrollWheelDelta() { return ScrollDelta; }

    bool IsMouseButtonDown(MouseButton button) { return MouseButtonsPressedThisFrame[(int)button]; }
    bool IsMouseButtonUp(MouseButton button) { return !MouseButtonsPressedThisFrame[(int)button]; }
    bool WasMouseButtonPressed(MouseButton button) { return MouseButtonsPressedThisFrame[(int)button] && !MouseButtonsPressedLastFrame[(int)button]; }
    bool WasMouseButtonReleased(MouseButton button) { return !MouseButtonsPressedThisFrame[(int)button] && MouseButtonsPressedLastFrame[(int)button]; }

    bool IsKeyDown(Key key) { return KeysPressedThisFrame[(int)key]; }
    bool IsKeyUp(Key key) { return !KeysPressedThisFrame[(int)key]; }
    bool WasKeyPressed(Key key) { return KeysPressedThisFrame[(int)key] && !KeysPressedLastFrame[(int)key]; }
    bool WasKeyReleased(Key key) { return !KeysPressedThisFrame[(int)key] && KeysPressedLastFrame[(int)key]; }

    bool IsCursorDisabled() { return glfwGetInputMode(Instance->Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED; }
    void SetCursorDisabled(bool disabled) { glfwSetInputMode(Instance->Window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }
    bool IsRawMouseMotionEnabled() { return glfwGetInputMode(Instance->Window, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE; }
    void SetRawMouseMotion(bool isRaw)
    {
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(Instance->Window, GLFW_RAW_MOUSE_MOTION, isRaw ? GLFW_TRUE : GLFW_FALSE);
    }

    void Update()
    {
        // Cursor
        OldMousePos = MousePos;

        double mouseX, mouseY;
        glfwGetCursorPos(Instance->Window, &mouseX, &mouseY);
        MousePos.x = (float)mouseX;
        MousePos.y = (float)(Instance->ScreenHeight - mouseY);

        if (!OldMousePosInitialized)
        {
            OldMousePosInitialized = true;
            OldMousePos = MousePos;
        }

        // Mouse buttons
        for (int i = 0; i < MouseButtonCount; ++i)
        {
            int id = GetGLFWMouseButtonCode((MouseButton)i);

            if (id == -1)
                continue;

            MouseButtonsPressedLastFrame[i] = MouseButtonsPressedThisFrame[i];
            MouseButtonsPressedThisFrame[i] = glfwGetMouseButton(Instance->Window, id) == GLFW_PRESS;
        }

        // Keys
        for (int i = 0; i < KeyCount; ++i)
        {
            int id = GetGLFWKeyCode((Key)i);

            if (id == -1)
                continue;

            KeysPressedLastFrame[i] = KeysPressedThisFrame[i];
            KeysPressedThisFrame[i] = glfwGetKey(Instance->Window, id) == GLFW_PRESS;
        }
    }

    void UpdateScroll(float xOffset, float yOffset)
    {
        ScrollDelta = yOffset;
    }

    void PostUpdate()
    {
        ScrollDelta = 0;
    }
}
