#pragma once

#include "Common.h"

#include "Version.h"
#include "LogManager.h"
#include "Graphics/Renderer.h"
#include "World/World.h"

namespace Minecraft
{
    class InputManager;

    inline shared_ptr<LogManager> Logger = nullptr;
    inline shared_ptr<InputManager> Input = nullptr;
    inline shared_ptr<class Renderer> Renderer = nullptr;
    inline shared_ptr<class World> World = nullptr;

    inline bool Running = true;
    inline std::thread::id MainThreadID = std::thread::id();
    inline shared_ptr<std::thread> TickThread = nullptr;

    // Application state: Do not modify! (apart from target loop rates)
    namespace Window
    {
        // User configurable settings
        inline float32 TargetFrameRate = 60;
        inline float32 TargetTickRate = 10;
        inline bool VSyncEnabled = false;

        // Updated as the application runs
        inline GLFWwindow* Handle = nullptr;
        inline int32 Width = 0;
        inline int32 Height = 0;

        inline void Close() { glfwSetWindowShouldClose(Window::Handle, true); }

        inline void SetVSyncEnabled(bool value)
        {
            glfwSwapInterval(value ? 1 : 0);
            VSyncEnabled = value;
        }
    }

    namespace Time
    {
        inline int32 UpdateCount = 0;
        inline int32 TickCount = 0;

        inline float32 DeltaTime = 0;
        inline float32 FixedDeltaTime = 0;

        inline float32 FrameRate = 0;
        inline float32 TickRate = 0;

        // Seconds since the program started - First is used in the update loop, second in the tick loop
        inline float32 WallTime = 0;
        inline float32 FixedWallTime = 0;

        inline chrono::time_point<chrono::steady_clock> StartTime;
    }
}
