#pragma once

#include "Common.h"
#include "LogManager.h"
#include "Graphics/Renderer.h"
#include "World/World.h"

namespace Minecraft
{
    class InputManager;

    extern shared_ptr<LogManager> Logger;
    extern shared_ptr<InputManager> Input;
    extern shared_ptr<Renderer> Renderer;
    extern shared_ptr<World> CurrentWorld;

    extern std::thread::id MainThreadID;
    extern shared_ptr<std::thread> TickThread;

    void Initialize();
    void Run();
    void Shutdown();

    namespace Window
    {
        const int32 InitialWidth = 1280;
        const int32 InitialHeight = 720;
        const string Title = "Minecraft Remake";
        const bool StartFullScreen = true;

        extern GLFWwindow* Handle;
        extern int32 Width;
        extern int32 Height;

        inline void Close()
        {
            glfwSetWindowShouldClose(Window::Handle, true);
        }
    }

    namespace Time
    {
        extern int32 UpdateCount;
        extern int32 TickCount;

        extern float32 DeltaTime;
        extern float32 FixedDeltaTime;

        extern float32 FrameRate;
        extern float32 TickRate;

        // Seconds since the program started
        extern float32 WallTime;
    }

    namespace Version
    {
        const extern int32 Major;
        const extern int32 Minor;
        const extern int32 Patch;
        const extern string String;
    }
}
