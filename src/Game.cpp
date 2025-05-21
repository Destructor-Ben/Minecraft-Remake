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
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Random/RandomTests.h"
#include "UI/UI.h"
#include "World/World.h"

namespace Minecraft
{
    #pragma region Initialization

    void Game::Initialize()
    {
        Logger = make_shared<LogManager>();
        Logger->Info(format("Starting Minecraft_Remake version {}...", Version::String));

        PerfProfiler = make_shared<Profiler>();
        PerfProfiler->BeginFrame("Load");

        PerfProfiler->Push("WindowInit");

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window);

        PerfProfiler->Pop();

        PerfProfiler->Push("MiscManagers");

        Input = make_shared<InputManager>();
        Resources = make_shared<ResourceManager>();

        m_TickPerfData = { };
        m_UpdatePerfData = { };
        m_RenderPerfData = { };

        PerfProfiler->Pop();

        PerfProfiler->Push("GraphicsInit");

        Graphics = make_shared<Renderer>();
        ChunkGraphics = make_shared<ChunkRenderer>();
        SkyGraphics = make_shared<SkyRenderer>();
        TextRenderer::Init();
        UI = make_shared<UIRenderer>();

        PerfProfiler->Pop();

        PerfProfiler->Push("UIInit");
        UI::Init();
        PerfProfiler->Pop();

        auto frameData = PerfProfiler->EndFrame();
        Logger->Debug("Debug load times:\n" + frameData.ToString());

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

        glfwTerminate();

        PerfProfiler = nullptr;
        Logger = nullptr;
    }

    void Game::InitGLFW()
    {
        PerfProfiler->Push("InitGLFW");

        Logger->Info("Initializing GLFW...");

        glfwSetErrorCallback(GLFWError);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #if DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        #endif

        PerfProfiler->Push("Creating Window");

        const int InitialWidth = 1280;
        const int InitialHeight = 720;
        ScreenWidth = InitialWidth;
        ScreenHeight = InitialHeight;
        Window = glfwCreateWindow(InitialWidth, InitialHeight, "Minecraft Remake", nullptr, nullptr);
        glfwMakeContextCurrent(Window);

        PerfProfiler->Pop();

        glfwSetFramebufferSizeCallback(Window, OnResize);
        glfwSetScrollCallback(Window, OnScroll);
        glfwSetInputMode(Window, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        SetVSyncEnabled(true);
        SetMouseHidden(false);

        Logger->Info("GLFW Initialized");

        PerfProfiler->Pop();
    }

    void Game::InitGL()
    {
        PerfProfiler->Push("InitGL");

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

        PerfProfiler->Pop();
    }

    #pragma endregion

    #pragma region Update Functions

    // TODO: also sometimes print all debug data if more chunks are generated on the same frame so I can tell if
    void Game::HandleProfilerData(const ProfilerData& data, Key debugKey, vector <ProfilerData>& previousData)
    {
        if (Input->WasKeyPressed(debugKey))
            Logger->Debug("\n" + data.ToString());

        if (Input->IsKeyDown(debugKey))
            previousData.push_back(data);

        if (Input->WasKeyReleased(debugKey))
        {
            previousData.push_back(data);

            // TODO: average the data
            auto averageData = data;

            //Logger->Debug("\n" + averageData.ToString());

            previousData.clear();
        }
    }

    void Game::Tick()
    {
        PerfProfiler->BeginFrame("Tick");

        if (CurrentWorld)
            CurrentWorld->Tick();

        auto data = PerfProfiler->EndFrame();
        HandleProfilerData(data, Key::LeftBracket, m_TickPerfData);
    }

    void Game::Update()
    {
        PerfProfiler->BeginFrame("Update");

        Input->Update();

        // World updating
        if (InGame && !IsPaused && CurrentWorld)
            CurrentWorld->Update();

        // UI updating
        UI->Update();

        Input->PostUpdate();

        auto data = PerfProfiler->EndFrame();
        HandleProfilerData(data, Key::RightBracket, m_UpdatePerfData);
    }

    void Game::Render()
    {
        PerfProfiler->BeginFrame("Render");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Graphics->PreRender();

        // World rendering
        if (CurrentWorld)
            CurrentWorld->Render();

        // UI rendering
        UI->Render();

        Graphics->PostRender();

        auto data = PerfProfiler->EndFrame();
        HandleProfilerData(data, Key::BackSlash, m_RenderPerfData);
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
            if (!IsPaused)
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

            if (IsPaused)
                TickDeltaTime = 0;

            Running = !glfwWindowShouldClose(Window);
        }
    }

    #pragma endregion

    #pragma region Window Control Functions

    void Game::Close()
    {
        glfwSetWindowShouldClose(Window, true);
    }

    void Game::SetVSyncEnabled(bool value)
    {
        glfwSwapInterval(value ? 1 : 0);
        m_VSyncEnabled = value;
    }

    void Game::SetMouseHidden(bool hidden)
    {
        m_IsMouseHidden = hidden;
        InputManager::SetRawMouseMotion(hidden);
        InputManager::SetCursorDisabled(hidden);
    }

    // TODO: should be in LogManager
    void Game::GLFWError(int code, cstring description)
    {
        Instance->Logger->Error(format("GLFW Error (Code {}): {}", code, description));
    }

    // TODO: should be in LogManager
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
        Instance->ScreenSize = vec2i(width, height);
        Instance->ScreenRect = Rectangle(0, 0, width, height);
    }

    #pragma endregion
}
