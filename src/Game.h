#pragma once

#include "Common.h"

#include "Input/Input.h"
#include "Graphics/Renderer.h"

namespace Minecraft
{
    extern InputManager* Input;
    extern Renderer* Camera;

    void Initialize();
    void Shutdown();

    void Tick(float deltaTime);
    void Update(float deltaTime);
    void Render();
}
