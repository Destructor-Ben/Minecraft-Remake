#include "Input.h"

#include "../Graphics/Window.h"

// TODO: make keybinds that can be reassigned
namespace Minecraft
{
    static const int KeyCount = (int)Key::Count;

    static int GetGLFWKeyCode(Key key)
    {
        switch (key)
        {
            // Numbers
            case Key::Zero:
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

            // Letters
            case Key::A:
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

            // Punctuation
            case Key::Grave:
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
            case Key::Space:
                return GLFW_KEY_SPACE;

            // Function keys
            // TODO: function keys

            // Unknown key
            default:
                return -1;
        }
    }

    InputManager::InputManager()
    {
        m_PressedThisFrame = new bool[KeyCount];
        m_PressedLastFrame = new bool[KeyCount];

        for (int i = 0; i < KeyCount; i++)
        {
            m_PressedThisFrame[i] = false;
            m_PressedLastFrame[i] = false;
        }
    }

    InputManager::~InputManager()
    {
        delete[] m_PressedThisFrame;
        delete[] m_PressedLastFrame;
    }

    glm::vec2 InputManager::GetMousePos() const
    {
        return m_MousePos;
    }

    bool InputManager::IsKeyDown(Key key) const
    {
        return m_PressedThisFrame[(int)key];
    }

    bool InputManager::IsKeyUp(Key key) const
    {
        return !m_PressedThisFrame[(int)key];
    }

    bool InputManager::WasKeyPressed(Key key) const
    {
        return m_PressedThisFrame[(int)key] && !m_PressedLastFrame[(int)key];
    }

    bool InputManager::WasKeyReleased(Key key) const
    {
        return !m_PressedThisFrame[(int)key] && m_PressedLastFrame[(int)key];
    }

    void InputManager::Update()
    {
        // Mouse
        double mouseX, mouseY;
        glfwGetCursorPos(Window::Handle, &mouseX, &mouseY);
        m_MousePos.x = (float)mouseX;
        m_MousePos.y = (float)mouseY;

        // Keys
        for (int i = 0; i < KeyCount; ++i) {
            int key = GetGLFWKeyCode((Key)i);

            if (key == -1)
                continue;

            m_PressedLastFrame[i] = m_PressedThisFrame[i];
            m_PressedThisFrame[i] = glfwGetKey(Window::Handle, key) == GLFW_PRESS;
        }
    }
}
