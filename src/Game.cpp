#include "Game.h"

namespace Minecraft
{
    // TODO: change the way pointers work - unique/shared ptrs for ownership, then references/raw pointers for accessing
    // TODO: use proper typedefs and namespaces, as well as sorted includes
    // TODO: use quaternions for rotation instead of euler angles
    // TODO: fix random crash from glm miscalculation because the window isn't focused - also make the window behave better when not selected
    // TODO: better error handling: wrap main loop in try catch to avoid exceptions from crashing the game - also make shader compilation errors vetter

    class Logger* Logger = nullptr;
    class InputManager* Input = nullptr;
    class Renderer* Renderer = nullptr;
    class World* World = nullptr;

    shared_ptr<std::thread> TickThread = nullptr;

    static void GLFWError(int32 code, cstring description)
    {
        // TODO: improve GLFW errors - Make error code better
        Logger->Warn(format("GLFW Error ({}): {}", code, description));
    }

    static void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam)
    {
        // TODO: improve GLErrors - https://www.khronos.org/opengl/wiki/Debug_Output
        // glDebugMessageInsert(1, 1, 1, 1, 5, "Test");
        Logger->Warn(format("GL Error:\n  Source: {}\n  Type: {}\n  ID: {}\n  Severity: {}\n  Message: {}", source, type, id, severity, message));
    }

    static void Resize(GLFWwindow* window, int32 width, int32 height)
    {
        glViewport(0, 0, width, height);
        Window::Width = width;
        Window::Height = height;
    }

    static void OnScroll(GLFWwindow* window, float64 xOffset, float64 yOffset)
    {
        Input->UpdateScroll((float32)xOffset, (float32)yOffset);
    }

    static void InitGLFW()
    {
        Logger->Info("Initializing GLFW...");

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        Window::Handle = glfwCreateWindow(Window::InitialWidth, Window::InitialHeight, Window::Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(Window::Handle);
        glfwSetErrorCallback(GLFWError);

        glfwSetFramebufferSizeCallback(Window::Handle, Resize);
        glfwSetInputMode(Window::Handle, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(Window::Handle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        glfwSetScrollCallback(Window::Handle, OnScroll);
        glfwSwapInterval(1); // TODO: fix frame rate properly

        Logger->Info("GLFW Initialized");
    }

    static void InitGL()
    {
        Logger->Info("Initializing OpenGL...");

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(GLError, nullptr);

        // Viewport settings
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, Window::InitialWidth, Window::InitialHeight);

        // Culling
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Blending TODO
        //glEnable(GL_BLEND);

        Logger->Info("OpenGL Initialized");
    }

    static void Tick()
    {
        World->Tick();
    }

    static void Update()
    {
        Input->Update();
        World->Update();
        Renderer->Update();

        Input->PostUpdate();
    }

    static void Render()
    {
        Renderer::Clear();

        World->Render();
    }

    static void RunTickLoop()
    {
        Logger->Info("Running tick thread...");

        const auto start = std::chrono::steady_clock::now();
        const auto period = std::chrono::seconds(1);

        while (!glfwWindowShouldClose(Window::Handle))
        {
            Tick();

            // TODO: delay properly
            Time::TickCount++;
            auto now = std::chrono::steady_clock::now();
            auto iterations = (now - start) / period;
            auto next_start = start + (iterations + 1) * period;
            std::this_thread::sleep_until(next_start);
        }

        Logger->Info("Exited tick thread");
    }

    void Initialize()
    {
        Logger = new class Logger();
        Logger->Info("Starting...");

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window::Handle);

        Input = new class InputManager();
        Renderer = new class Renderer();
        Renderer->ChunkRenderer = new ChunkRenderer(); // TODO: maybe make a prepare function
        World = new class World();

        Renderer::UnbindAll();
    }

    void Run()
    {
        // TODO: tick thread - https://stackoverflow.com/questions/52260084/how-to-maintain-certain-frame-rate-in-different-threads
        // TODO: fix frame rate

        TickThread = make_shared<std::thread>(RunTickLoop);

        Logger->Info("Running window...");

        while (!glfwWindowShouldClose(Window::Handle))
        {
            Time::WallTime = (float32)glfwGetTime();

            Update();
            Render();

            // TODO: this is wrong, maybe do the calculations at the beginning?
            Time::DeltaTime = (float32)glfwGetTime() - Time::WallTime;
            Time::FrameRate = 1.0f / Time::DeltaTime;
            Time::UpdateCount++;

            glfwSwapBuffers(Window::Handle);
            glfwPollEvents();
        }
    }

    void Shutdown()
    {
        Logger->Info("Waiting for tick thread to exit...");
        TickThread->join();

        Logger->Info("Exiting...");

        Renderer::UnbindAll();

        delete World;
        delete Renderer;
        delete Input;

        glfwTerminate();

        delete Logger;
    }
}
