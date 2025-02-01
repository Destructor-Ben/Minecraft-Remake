#pragma once

// TODO: color utils to convert to/from hex and rgb
namespace Minecraft
{
    class LogManager;
    class InputManager;
    class ResourceManager;
    class Renderer;
    class ChunkRenderer;
    class SkyRenderer;
    class World;

    class Game;

    inline shared_ptr<Game> Instance = nullptr;

    class Game
    {
    public:
        // Loop variables
        bool Running = true;
        chrono::time_point<chrono::steady_clock> StartTime;
        float ElapsedSeconds = 0;

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

        // Managers
        shared_ptr<LogManager> Logger = nullptr;
        shared_ptr<InputManager> Input = nullptr;
        shared_ptr<ResourceManager> Resources = nullptr;

        shared_ptr<Renderer> Graphics = nullptr;
        shared_ptr<ChunkRenderer> ChunkGraphics = nullptr;
        shared_ptr<SkyRenderer> SkyGraphics = nullptr;

        shared_ptr<World> CurrentWorld = nullptr;

        void Run();
        void Initialize();
        void Shutdown();

        void Tick();
        void Update();
        void Render();

        float GetFrameRate() const { return 1.0 / DeltaTime; }
        float GetTickRate() const { return 1.0 / TickDeltaTime; }

        void Close() const { glfwSetWindowShouldClose(Window, true); }

        bool IsVSyncEnabled() const { return m_VSyncEnabled; }
        void SetVSyncEnabled(bool value)
        {
            glfwSwapInterval(value ? 1 : 0);
            m_VSyncEnabled = value;
        }

    private:
        void InitGL();
        void InitGLFW();

        static void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam);
        static void GLFWError(int code, cstring description);

        static void OnScroll(GLFWwindow* window, double xOffset, double yOffset);
        static void OnResize(GLFWwindow* window, int width, int height);

        bool m_VSyncEnabled = false;
    };
}
