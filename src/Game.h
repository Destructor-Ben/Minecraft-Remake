#pragma once

#include "Profiler.h"
#include "Rectangle.h"
#include "Input/Key.h"

// TODO: proper name, namespace, and logo
// TODO: make all managers have an init function instead of doing stuff in ctors
// - This is because sometimes they will need to refer to Instance->XXManager to init themselves, but the object exists, but hasn't been assigned to a variable yet
// - If I use an init function, then all managers can be stored by value (no shared_ptr) because they won't init anything until Init is called, so the init order is still deterministic
// TODO: loading screens + better init order
// TODO: rename all managers
// TODO: better include management - including this file should include all managers
// - To opt out for the managers header files themselves, allow defining an include to disable it!
// - E.g. #ifndef DONT_INCLUDE_MANAGERS {includes} #endif and #define DONT_INCLUDE_MANAGERS to out out
// TODO: can't this just be a static class? is there any reason for this to be a singleton? does it even need to be a class? could it just be a namespace?
// - After looking online, just make a namespace
// TODO: minimum window size
// TODO: ensure graceful shutdown
// - Make sure all of the system signals are respected
//   - SIGINT
//   - SIGTERM
//   - SIGKILL
//   - Any others for killing processes
// - Error message dialog box if there is an error as well as print to error log
// - Also allow segfaults to give an error message - I think its also a process signal
// - Ctrl + c interrupting
// TODO: better game loops
// TODO: multithread loading and initial world generation to stop the main thread being blocked (it shouldn't be too hard right?)
// TODO: debug drawing for rectangles/bounds with UI
// TODO: for all color stuff, use Color
namespace Minecraft
{
    class Renderer;
    class ChunkRenderer;
    class SkyRenderer;
    class World;

    class Game;

    inline shared_ptr <Game> Instance = nullptr;

    class Game
    {
    public:
        // Loop variables
        bool Running = true;
        chrono::time_point <chrono::steady_clock> StartTime;
        float ElapsedSeconds = 0;

        bool IsPaused = true;
        bool InGame = false;

        int UpdateCount = 0;
        int TickCount = 0;

        float DeltaTime = 0;
        float TickDeltaTime = 0;

        float TargetFrameRate = 60;
        float TargetTickRate = 10;

        // Window
        GLFWwindow* Window = nullptr;
        int ScreenWidth = 0;
        int ScreenHeight = 0;
        vec2i ScreenSize = { };
        Rectangle ScreenRect = { };

        // Profilers
        // TODO: make general purpose profiler and profiler for tick thread
        shared_ptr <Profiler> PerfProfiler = nullptr;

        // Renderers
        shared_ptr <Renderer> Graphics = nullptr;
        shared_ptr <ChunkRenderer> ChunkGraphics = nullptr;
        shared_ptr <SkyRenderer> SkyGraphics = nullptr;

        // World
        shared_ptr <World> CurrentWorld = nullptr;

        void Run();
        void Initialize();
        void Shutdown();

        void Tick();
        void Update();
        void Render();

        void Close();

        bool IsVSyncEnabled() const { return m_VSyncEnabled; }
        void SetVSyncEnabled(bool value);

        bool IsMouseHidden() const { return m_IsMouseHidden; }
        void SetMouseHidden(bool hidden);

    private:
        void InitGL();
        void InitGLFW();

        static void OnScroll(GLFWwindow* window, double xOffset, double yOffset);
        static void OnResize(GLFWwindow* window, int width, int height);

        bool m_VSyncEnabled = false;
        bool m_IsMouseHidden = false;

        GLFWcursor* m_LightCursor;
        GLFWcursor* m_DarkCursor;
    };
}
