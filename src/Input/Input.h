#pragma once

#include "../Common.h"

#include "Key.h"

namespace Minecraft
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        glm::vec2 GetMousePos() const;
        // TODO: mouse buttons - could just implement as a key internally and have special functions for them

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
