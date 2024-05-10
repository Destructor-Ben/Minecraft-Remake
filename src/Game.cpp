#include "Game.h"

#include "Input/InputManager.h"

namespace Minecraft
{
    // TODO: use correct typedefs, ensure namespaces, sort includes, and use smart pointers for ownership
    // TODO: make Game.h included in Common.h, also make Game.h include common files
    // TODO: embed resources - use source generator https://stackoverflow.com/a/71906177/12259381

    // TODO: use quaternions for rotation instead of euler angles
    // TODO: blending
    // TODO: fix frame rate properly, time calculations, and allow options for changing frame rate and tick rate

    // TODO: fix random crash from glm miscalculation because the window isn't focused - also make the window behave better when not selected - possibly already fixed in camera.cpp

    shared_ptr<LogManager> Logger = nullptr;
    shared_ptr<InputManager> Input = nullptr;
    shared_ptr<class Renderer> Renderer = nullptr;
    shared_ptr<class World> World = nullptr;

    bool Running = true;
    std::thread::id MainThreadID = std::thread::id();
    shared_ptr<std::thread> TickThread = nullptr;

    static void GLFWError(int32 code, cstring description)
    {
        Logger->Error(format("GLFW Error (Code {}): {}", code, description));
    }

    static void GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam)
    {
        string sourceString;
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                sourceString = "API";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                sourceString = "Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                sourceString = "Other";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                sourceString = "Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                sourceString = "Third Party";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                sourceString = "Window System";
                break;
            default:
                Logger->Throw("Unknown GL Error source: " + to_string(source));
                break;
        }

        string typeString;
        switch (type)
        {
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeString = "Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_ERROR:
                typeString = "Error";
                break;
            case GL_DEBUG_TYPE_MARKER:
                typeString = "Marker";
                break;
            case GL_DEBUG_TYPE_OTHER:
                typeString = "Other";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                typeString = "Performance";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                typeString = "Pop Group";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                typeString = "Portability";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                typeString = "Push Group";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeString = "Undefined Behaviour";
                break;
            default:
                Logger->Throw("Unknown GL Error type: " + to_string(source));
                break;
        }

        string severityString;
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                severityString = "High";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                severityString = "Medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                severityString = "Low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                severityString = "Notification";
                break;
            default:
                Logger->Throw("Unknown GL Error severity: " + to_string(source));
                break;
        }

        string logMessage = format("GL Error: {}\n  Source: {}\n  Type: {}\n  Severity: {}\n  ID: {}", message, sourceString, typeString, severityString, id);

        if (severity == GL_DEBUG_SEVERITY_HIGH)
            Logger->Error(logMessage);
        else
            Logger->Warn(logMessage);
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

        glfwSetErrorCallback(GLFWError);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        Window::Handle = glfwCreateWindow(Window::InitialWidth, Window::InitialHeight, Window::Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(Window::Handle);

        glfwSetFramebufferSizeCallback(Window::Handle, Resize);
        glfwSetScrollCallback(Window::Handle, OnScroll);
        glfwSetInputMode(Window::Handle, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(Window::Handle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        Window::SetVSyncEnabled(true);

        Logger->Info("GLFW Initialized");
    }

    static void InitGL()
    {
        Logger->Info("Initializing OpenGL...");

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#if DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLError, nullptr);
#endif

        // Viewport settings
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, Window::InitialWidth, Window::InitialHeight);

        // Culling
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

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

        while (Running)
        {
            Tick();

            // TODO: delay properly
            auto now = std::chrono::steady_clock::now();
            auto iterations = (now - start) / period;
            auto next_start = start + (iterations + 1) * period;
            std::this_thread::sleep_until(next_start);
            Time::TickCount++;
        }

        Logger->Info("Exited tick thread");
    }

    void Initialize()
    {
        MainThreadID = std::this_thread::get_id(); // TODO: seems to cause segfault in std::thread::id very rarely
        Logger = make_shared<LogManager>();
        Logger->Info(format("Starting Minecraft_Remake version {}...", Version::String));

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window::Handle);

        Input = make_shared<InputManager>();
        Renderer = make_shared<class Renderer>();
        Renderer->ChunkRenderer = make_shared<ChunkRenderer>(); // TODO: maybe make a prepare function
        World = make_shared<class World>();

        Renderer::UnbindAll();
    }

    void Run()
    {
        // TODO: tick thread - https://stackoverflow.com/questions/52260084/how-to-maintain-certain-frame-rate-in-different-threads
        // TODO: fix frame rate

        TickThread = make_shared<std::thread>(RunTickLoop);

        Logger->Info("Running window...");

        while (Running)
        {
            Time::WallTime = (float32)glfwGetTime();

            Update();
            Render();

            // TODO: this is wrong, maybe do the calculations at the beginning?
            Time::DeltaTime = (float32)glfwGetTime() - Time::WallTime;
            Time::FrameRate = 1.0f / Time::DeltaTime;
            Time::UpdateCount++;

            Running = !glfwWindowShouldClose(Window::Handle);
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

        // We manually null these out because we need to deallocate the objects in a guaranteed order
        World = nullptr;
        Renderer = nullptr;
        Input = nullptr;

        glfwTerminate();

        Logger = nullptr;
    }

    namespace Window
    {
        float32 TargetFrameRate = 60.0f;
        float32 TargetTickRate = 10.0f;
        bool FullScreen = true;
        bool VSyncEnabled = false;

        GLFWwindow* Handle = nullptr;
        int32 Width = 0;
        int32 Height = 0;
    }

    namespace Time
    {
        int32 UpdateCount = 0;
        int32 TickCount = 0;

        float32 DeltaTime = 0;
        float32 FixedDeltaTime = 0;

        float32 FrameRate = 0;
        float32 TickRate = 0;

        float32 WallTime = 0;
    }

    namespace Version
    {
        const string String = format("{}.{}.{}", Major, Minor, Patch);
    }
}
