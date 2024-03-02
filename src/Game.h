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
    extern Renderer* Camera;
    extern World* CurrentWorld; // TODO: consider renaming

    void Initialize();
    void Shutdown();

    void Tick();
    void Update();
    void Render();
}
