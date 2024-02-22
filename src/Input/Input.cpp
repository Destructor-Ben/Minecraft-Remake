#include "Input.h"

#include "../Game.h"

namespace Minecraft
{
    bool IsKeyDown(Key key)
    {
        return pressedThisFrame[(int)key];
    }

    bool IsKeyUp(Key key)
    {
        return !pressedThisFrame[(int)key];
    }

    bool WasKeyPressed(Key key)
    {
        return pressedThisFrame[(int)key] && !pressedLastFrame[(int)key];
    }

    bool WasKeyReleased(Key key)
    {
        return !pressedThisFrame[(int)key] && pressedLastFrame[(int)key];
    }

    void InitializeInput() {
        int count = (int)Key::Count;

        pressedThisFrame = new bool[count];
        pressedLastFrame = new bool[count];

        for (int i = 0; i < count; ++i) {
            pressedThisFrame[i] = false;
            pressedLastFrame[i] = false;
        }
    }

    void ShutdownInput() {
        delete[] pressedThisFrame;
        delete[] pressedLastFrame;
    }

    void UpdateInput()
    {
        int count = (int)Key::Count;

        for (int i = 0; i < count; ++i) {
            int key = GetGLFWKeyCode((Key)i);

            if (key == -1)
                continue;

            pressedLastFrame[i] = pressedThisFrame[i];
            pressedThisFrame[i] = glfwGetKey(window, key) == GLFW_PRESS;
        }
    }

    int GetGLFWKeyCode(Key key) {
        // TODO: implement this fully
        switch (key) {
            case Key::Space:
                return GLFW_KEY_SPACE;
            case Key::A:
                return GLFW_KEY_A;
            default:
                return -1;
        }
    }
}
