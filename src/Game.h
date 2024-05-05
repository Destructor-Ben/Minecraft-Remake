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
    extern shared_ptr<Logger> Logger;
    extern shared_ptr<Renderer> Renderer;
    extern shared_ptr<World> World;
    extern shared_ptr<InputManager> Input;

    extern std::thread::id MainThreadID;
    extern shared_ptr<std::thread> TickThread;

    void Initialize();
    void Run();
    void Shutdown();
}
