#pragma once

#include "Common.h"

#include "Input/Input.h"

namespace Minecraft
{
    extern InputManager* Input;

    void Initialize();
    void Shutdown();

    void Tick(float deltaTime);
    void Update(float deltaTime);
    void Render();
}
