#pragma once

#include "Common.h"
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

    // Application state: Do not modify!
    namespace Window
    {
        // Initial settings
        const int32 InitialWidth = 1280;
        const int32 InitialHeight = 720;
        const string Title = "Minecraft Remake";

        // User configurable settings - TODO - use TargetFrameRate
        inline float32 TargetFrameRate = 60.0f;
        inline float32 TargetTickRate = 10.0f;
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

        // Seconds since the program started
        inline float32 WallTime = 0;
    }

    namespace Version
    {
        // TODO: allow these to be const, also remove the library from CMakeLists.txt and just add to source files
        extern int32 Major;
        extern int32 Minor;
        extern int32 Patch;
        inline const string String = format("{}.{}.{}", Major, Minor, Patch);
    }
}
