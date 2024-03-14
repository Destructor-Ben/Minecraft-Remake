#pragma once

#include "Common.h"

#include "Time.h"
#include "World/World.h"
#include "Input/InputManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

namespace Minecraft
{
    extern InputManager* Input;
    extern Renderer* Renderer;
    extern World* World;

    void Initialize();
    void Shutdown();

    void Tick();
    void Update();
    void Render();
}
