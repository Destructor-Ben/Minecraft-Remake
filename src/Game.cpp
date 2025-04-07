#include "Game.h"

#include "LogManager.h"
#include "Profiler.h"
#include "ResourceManager.h"
#include "Version.h"
#include "Input/InputManager.h"
#include "Graphics/GL.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/SkyRenderer.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Random/RandomTests.h"
#include "World/World.h"

namespace Minecraft
{
    #pragma region Initialization

    void Game::Initialize()
    {
        Logger = make_shared<LogManager>();
        Logger->Info(format("Starting Minecraft_Remake version {}...", Version::String));

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window);

        Input = make_shared<InputManager>();
        Resources = make_shared<ResourceManager>();
        PerfProfiler = make_shared<Profiler>();

        Graphics = make_shared<Renderer>();
        ChunkGraphics = make_shared<ChunkRenderer>();
        SkyGraphics = make_shared<SkyRenderer>();
        UI = make_shared<UIRenderer>();

        CurrentWorld = make_shared<World>();

        // Running tests
        // Uncomment to run them
        // RunRandomTests();
    }

    void Game::Shutdown()
    {
        Logger->Info("Exiting...");

        // We manually null these out because we need to deallocate the objects in a guaranteed order
        CurrentWorld = nullptr;
        ChunkGraphics = nullptr;
        Graphics = nullptr;
        Resources = nullptr;
        Input = nullptr;
        PerfProfiler = nullptr;

        glfwTerminate();

        Logger = nullptr;
    }

    void Game::InitGLFW()
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
        ScreenWidth = InitialWidth;
        ScreenHeight = InitialHeight;
        Window = glfwCreateWindow(InitialWidth, InitialHeight, "Minecraft Remake", nullptr, nullptr);
        glfwMakeContextCurrent(Window);

        glfwSetFramebufferSizeCallback(Window, OnResize);
        glfwSetScrollCallback(Window, OnScroll);
        glfwSetInputMode(Window, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        SetVSyncEnabled(true);

        Logger->Info("GLFW Initialized");
    }

    void Game::InitGL()
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
        glViewport(0, 0, ScreenWidth, ScreenHeight);

        // Culling
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        Logger->Info("OpenGL Initialized");
    }

    #pragma endregion

    #pragma region Update Functions

    void Game::Tick()
    {
        PerfProfiler->BeginFrame("Tick");

        if (CurrentWorld)
            CurrentWorld->Tick();

        auto data = PerfProfiler->EndFrame();
        if (Input->WasKeyPressed(Key::LeftBracket))
            Logger->Debug("\n" + data.ToString());
    }

    void Game::Update()
    {
        PerfProfiler->BeginFrame("Update");

        Input->Update();

        if (CurrentWorld)
            CurrentWorld->Update();

        Input->PostUpdate();

        auto data = PerfProfiler->EndFrame();
        if (Input->WasKeyPressed(Key::RightBracket))
            Logger->Debug("\n" + data.ToString());
    }

    void Game::Render()
    {
        PerfProfiler->BeginFrame("Render");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // World rendering
        Graphics->PreRender();

        if (CurrentWorld)
            CurrentWorld->Render();

        Graphics->PostRender();

        // UI rendering
        // TODO: temporary, just testing
        // TODO: make a proper UI system
        // TODO: how to handle z values with sprites? default to 1? or just ignore and use render order for what's on top?
        Graphics->SceneCamera = &UI->UICamera;
        auto sprite = Sprite();
        sprite.SpriteTexture = Resources->RequestTexture("ui/crosshair");
        // TODO: fix scaling being dodgy, maybe use a downsized ui viewport
        sprite.Scale = vec3(sprite.SpriteTexture->GetWidth() * 3);
        sprite.Position = vec3(ScreenWidth / 2, ScreenHeight / 2, 0);
        UI->DrawSprite(sprite);

        auto data = PerfProfiler->EndFrame();
        if (Input->WasKeyPressed(Key::BackSlash))
            Logger->Debug("\n" + data.ToString());
    }

    #pragma endregion

    #pragma region Loops

    void Game::Run()
    {
        glfwSetTime(0);
        StartTime = chrono::steady_clock::now();
        Logger->Info("Running window...");

        while (Running)
        {
            Tick();
            Update();
            Render();

            // Manually handle delays if we don't have VSync
            if (!IsVSyncEnabled())
            {
                // There will likely be inaccuracies with the loop delays, so be careful when adjusting this code
                double iterations = glfwGetTime() * TargetFrameRate;
                double nextStart = (iterations + 1) / TargetFrameRate;
                std::this_thread::sleep_until(StartTime + chrono::duration<double>(nextStart));
            }

            glfwSwapBuffers(Window);
            glfwPollEvents();

            DeltaTime = (float)glfwGetTime() - ElapsedSeconds;
            ElapsedSeconds = (float)glfwGetTime();
            UpdateCount++;

            // Temporary until I make ticking separate to updates
            TickCount = UpdateCount;
            TickDeltaTime = DeltaTime;

            Running = !glfwWindowShouldClose(Window);
        }
    }

    #pragma endregion

    #pragma region Callbacks

    void Game::GLFWError(int code, cstring description)
    {
        Instance->Logger->Error(format("GLFW Error (Code {}): {}", code, description));
    }

    void Game::GLError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, cstring message, const void* userParam)
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
                Instance->Logger->Throw("Unknown GL Error source: " + to_string(source));
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
                Instance->Logger->Throw("Unknown GL Error type: " + to_string(source));
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
                Instance->Logger->Throw("Unknown GL Error severity: " + to_string(source));
                break;
        }

        string logMessage = format("GL Error: {}\n  Source: {}\n  Type: {}\n  Severity: {}\n  ID: {}", message, sourceString, typeString, severityString, id);

        if (severity == GL_DEBUG_SEVERITY_HIGH)
            Instance->Logger->Error(logMessage);
        else
            Instance->Logger->Warn(logMessage);
    }

    void Game::OnScroll(GLFWwindow* window, double xOffset, double yOffset)
    {
        Instance->Input->UpdateScroll((float)xOffset, (float)yOffset);
    }

    void Game::OnResize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Instance->ScreenWidth = width;
        Instance->ScreenHeight = height;
    }

    #pragma endregion
}
