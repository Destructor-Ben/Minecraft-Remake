#include "Game.h"

#include "Logger.h"
#include "Profiler.h"
#include "ResourceManager.h"
#include "Version.h"
#include "Input/Input.h"
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
        Logger::Init();
        Logger::Info(format("Starting Minecraft_Remake version {}...", Version::String));

        PerfProfiler = make_shared<Profiler>();
        PerfProfiler->BeginFrame("Init");

        PerfProfiler->Push("WindowInit");

        InitGLFW();
        InitGL();
        stbi_set_flip_vertically_on_load(true);
        glfwMaximizeWindow(Window);

        PerfProfiler->Pop();

        PerfProfiler->Push("MiscManagers");

        UIRenderer::Init();
        TextRenderer::Init();
        UI::Init();

        PerfProfiler->Pop();

        PerfProfiler->Push("GraphicsInit");

        Graphics = make_shared<Renderer>();
        ChunkGraphics = make_shared<ChunkRenderer>();
        SkyGraphics = make_shared<SkyRenderer>();

        PerfProfiler->Pop();

        auto loadTimeData = PerfProfiler->EndFrame();
        Logger::Debug("Load times:\n" + Profiler::ToString(loadTimeData));

        // Running tests
        // Uncomment to run them
        // RunRandomTests();
    }

    void Game::Shutdown()
    {
        Logger::Info("Exiting game...");

        // We manually null these out because we need to deallocate the objects in a guaranteed order
        CurrentWorld = nullptr;
        ChunkGraphics = nullptr;
        Graphics = nullptr;
        PerfProfiler = nullptr;

        glfwTerminate();
        Logger::Shutdown();
    }

    void Game::InitGLFW()
    {
        PerfProfiler->Push("InitGLFW");

        Logger::Info("Initializing GLFW...");

        glfwSetErrorCallback(Logger::GLFWError);

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

        PerfProfiler->Push("Setting Window Icon");

        // Set window icon
        auto iconTexture = Resources::RequestImageData("application/icon");
        GLFWimage icon[1];
        icon[0].width = iconTexture.Width;
        icon[0].height = iconTexture.Height;
        icon[0].pixels = iconTexture.Data.get();
        glfwSetWindowIcon(Window, 1, icon);

        PerfProfiler->Pop();

        PerfProfiler->Push("Creating Cursors");

        // Set cursor
        auto lightCursorTexture = Resources::RequestImageData("application/cursor-light");
        auto darkCursorTexture = Resources::RequestImageData("application/cursor-dark");
        GLFWimage lightCursor, darkCursor;
        lightCursor.width = lightCursorTexture.Width;
        lightCursor.height = lightCursorTexture.Height;
        lightCursor.pixels = lightCursorTexture.Data.get();
        darkCursor.width = darkCursorTexture.Width;
        darkCursor.height = darkCursorTexture.Height;
        darkCursor.pixels = darkCursorTexture.Data.get();
        m_LightCursor = glfwCreateCursor(&lightCursor, 0, 0);
        m_DarkCursor = glfwCreateCursor(&darkCursor, 0, 0);
        // TODO: update cursor when UI color mode changes
        glfwSetCursor(Window, UI::IsInLightMode ? m_LightCursor : m_DarkCursor);

        PerfProfiler->Pop();

        Logger::Info("GLFW Initialized");

        PerfProfiler->Pop();
    }

    void Game::InitGL()
    {
        PerfProfiler->Push("InitGL");

        Logger::Info("Initializing OpenGL...");

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        #if DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(Logger::GLError, nullptr);
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

        Logger::Info("OpenGL Initialized");

        PerfProfiler->Pop();
    }

    #pragma endregion

    #pragma region Update Functions

    void Game::Tick()
    {
        if (!InGame || IsPaused || CurrentWorld == nullptr)
            return;

        PerfProfiler->BeginFrame("Tick");

        CurrentWorld->Tick();

        Profiler::RecordFrameOrTickRate(false);

        auto data = PerfProfiler->EndFrame();
        Profiler::HandleProfilerData(data, ProfilerTarget::Tick);
    }

    void Game::Update()
    {
        PerfProfiler->BeginFrame("Update");

        Input::Update();

        // World updating
        if (InGame && !IsPaused && CurrentWorld)
            CurrentWorld->Update();

        // UI updating
        UIRenderer::Update();

        Input::PostUpdate();

        auto data = PerfProfiler->EndFrame();
        Profiler::HandleProfilerData(data, ProfilerTarget::Update);
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
        UIRenderer::Render();

        Graphics->PostRender();

        Profiler::RecordFrameOrTickRate(true);

        auto data = PerfProfiler->EndFrame();
        Profiler::HandleProfilerData(data, ProfilerTarget::Render);
    }

    #pragma endregion

    #pragma region Loops

    void Game::Run()
    {
        Logger::Info("Running window...");
        glfwSetTime(0);
        StartTime = chrono::steady_clock::now();

        while (Running)
        {
            Tick(); // Eventually tick will run on a separate thread
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
            // End temporary

            Running = !glfwWindowShouldClose(Window);
        }
    }

    #pragma endregion

    #pragma region World Management

    // TODO: queue the creation and exit of worlds so they don't happen mid update

    void Game::CreateAndEnterWorld(ulong seed)
    {
        Logger::Info(format("Creating world with seed: {}", seed));
        CurrentWorld = make_shared<World>(seed);

        // TODO: in future, all stuff below should be executed on another thread to avoid blocking the main one
        Logger::Info("Generating world...");
        CurrentWorld->Generate();

        Logger::Info("Entering world...");
        InGame = true;
        IsPaused = false;
        SetMouseHidden(true);
        CurrentWorld->OnEnter();
    }

    void Game::ExitCurrentWorld()
    {
        Logger::Info("Exiting world...");
        CurrentWorld->OnExit();
        CurrentWorld = nullptr;
        InGame = false;
        IsPaused = true;
        SetMouseHidden(false);
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
        Input::SetRawMouseMotion(hidden);
        Input::SetCursorDisabled(hidden);
    }

    void Game::OnScroll(GLFWwindow* window, double xOffset, double yOffset)
    {
        Input::UpdateScroll((float)xOffset, (float)yOffset);
    }

    void Game::OnResize(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Instance->ScreenWidth = width;
        Instance->ScreenHeight = height;
        Instance->ScreenSize = vec2i(width, height);
        Instance->ScreenRect = Rectangle(0, 0, width, height);
        UIRenderer::OnResize();
    }

    #pragma endregion
}
