#pragma once

#include "Common.h"

#include "Time.h"
#include "Input/Input.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

namespace Minecraft
{
    extern InputManager* Input;
    extern Renderer* Camera;

    void Initialize();
    void Shutdown();

    void Tick();
    void Update();
    void Render();
}
