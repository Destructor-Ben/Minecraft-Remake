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

    // TODO: make these inline globals
    namespace Window
    {
        // Initial settings
        const int32 InitialWidth = 1280;
        const int32 InitialHeight = 720;
        const string Title = "Minecraft Remake";

        // User configurable settings - TODO
        extern float32 TargetFrameRate;
        extern float32 TargetTickRate;
        extern bool FullScreen;
        extern bool VSyncEnabled; // Do not modify!

        // Updated as the application runs
        extern GLFWwindow* Handle;
        extern int32 Width;
        extern int32 Height;

        inline void Close() { glfwSetWindowShouldClose(Window::Handle, true); }

        inline void SetVSyncEnabled(bool value)
        {
            glfwSwapInterval(value ? 1 : 0);
            VSyncEnabled = value;
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

        extern float32 WallTime; // Seconds since the program started
    }

    namespace Version
    {
        // TODO: allow these to be const, also remove the library from CMakeLists.txt and just add to source files
        extern int32 Major;
        extern int32 Minor;
        extern int32 Patch;
        extern const string String;
    }
}
