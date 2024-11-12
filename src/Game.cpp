#include "Game.h"

#include "LogManager.h"
#include "Timers.h"
#include "Version.h"
#include "Window.h"
#include "Input/InputManager.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/World.h"

namespace Minecraft
{
    // TODO: embed resources - use source generator https://stackoverflow.com/a/71906177/12259381
    // TODO: fix the hierarchy structure of objects
    // TODO: blending
    // TODO: test loops properly
    // TODO: the tick loop needs to be on the same thread as the update loop
    // TODO: make a resourcemanager class

    #pragma region Resources

    string ReadResourceText(string path)
    {
        std::ifstream stream(path);
        if (stream.fail())
            Logger->Throw("Failed to load resource at path: " + path);

        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        return buffer.str();
    }

    vector <uint8> ReadResourceBytes(string path)
    {
        std::ifstream stream(path, std::ios::binary);
        if (stream.fail())
            Logger->Throw("Failed to load resource at path: " + path);

        vector <uint8> bytes((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        stream.close();
        return bytes;
    }

    #pragma endregion

    #pragma region Callbacks

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

    static void OnScroll(GLFWwindow* window, double xOffset, double yOffset)
    {
        Input->UpdateScroll((float)xOffset, (float)yOffset);
    }

    static void Resize(GLFWwindow* window, int32 width, int32 height)
    {
        glViewport(0, 0, width, height);
        Window::Width = width;
        Window::Height = height;
    }

    #pragma endregion

    #pragma region Initialization

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

        const int InitialWidth = 1280;
        const int InitialHeight = 720;
        Window::Width = InitialWidth;
        Window::Height = InitialHeight;
        Window::Handle = glfwCreateWindow(InitialWidth, InitialHeight, "Minecraft Remake", nullptr, nullptr);
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
        glViewport(0, 0, Window::Width, Window::Height);

        // Culling
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        Logger->Info("OpenGL Initialized");
    }

    void Initialize()
    {
        // Getting the thread id rarely causes a segfault in std::thread::id, might need investigation
        MainThreadID = std::this_thread::get_id();
        Logger = make_shared<LogManager>();
        Logger->Info(format("Starting Minecraft_Remake version {}...", Version::String));

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window::Handle);

        Input = make_shared<InputManager>();
        Renderer = make_shared<class Renderer>();
        World = make_shared<class World>();

        Renderer::UnbindAll();
    }

    void Shutdown()
    {
        Logger->Info("Waiting for tick thread to exit...");
        TickThread->join();
        TickThread = nullptr;

        Logger->Info("Exiting...");

        Renderer::UnbindAll();

        // We manually null these out because we need to deallocate the objects in a guaranteed order
        World = nullptr;
        Renderer = nullptr;
        Input = nullptr;

        glfwTerminate();

        Logger = nullptr;
    }

    #pragma endregion

    #pragma region Main Loops

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

    // There will likely be inaccuracies with the loop delays, so be careful when adjusting this code
    #define LOOP_DELAY(tickRate) \
        double iterations = glfwGetTime() * tickRate;\
        double nextStart = (iterations + 1) / tickRate;\
        std::this_thread::sleep_until(Timers::StartTime + chrono::duration<double>(nextStart));

    #define UPDATE_LOOP_VARIABLES(wallTime, deltaTime, loopRate, count) \
        deltaTime = (float)glfwGetTime() - wallTime;\
        wallTime = (float)glfwGetTime();\
        loopRate = 1.0f / deltaTime;\
        count++;

    static void RunTickLoop()
    {
        Logger->Info("Running tick thread...");

        while (Running)
        {
            Tick();

            LOOP_DELAY(Window::TargetTickRate)
            UPDATE_LOOP_VARIABLES(Timers::FixedWallTime, Timers::FixedDeltaTime, Timers::TickRate, Timers::TickCount)
        }

        Logger->Info("Exited tick thread");
    }

    void Run()
    {
        glfwSetTime(0);
        Timers::StartTime = chrono::steady_clock::now();
        TickThread = make_shared<std::thread>(RunTickLoop);
        Logger->Info("Running window...");

        while (Running)
        {
            Update();
            Render();

            if (!Window::VSyncEnabled)
            {
                LOOP_DELAY(Window::TargetFrameRate)
            }

            glfwSwapBuffers(Window::Handle);
            glfwPollEvents();

            UPDATE_LOOP_VARIABLES(Timers::WallTime, Timers::DeltaTime, Timers::FrameRate, Timers::UpdateCount)

            Running = !glfwWindowShouldClose(Window::Handle);
        }
    }

    #pragma endregion
}
