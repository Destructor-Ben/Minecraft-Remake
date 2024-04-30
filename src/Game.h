#pragma once

#include "Common.h"
#include "Time.h"
#include "Logger.h"
#include "World/World.h"
#include "Input/InputManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

namespace Minecraft
{
    extern Logger* Logger;
    extern InputManager* Input;
    extern Renderer* Renderer;
    extern World* World;

    extern shared_ptr<std::thread> TickThread;

    void Initialize();
    void Run();
    void Shutdown();
}
