#include "Common.h"

#include "Game.h"
#include "Time.h"
#include "Graphics/Window.h"

using namespace Minecraft;

// TODO: improve error checking at some point and also make logging better
// TODO: tick thread
// TODO: make the renderer take ownership of graphics resources and dispose of them automatically
// TODO: use quaternions for rotation instead of euler angles
// TODO: make root src an include dir so we can #include "Common.cpp" easily - https://stackoverflow.com/questions/32773283/cmake-include-header-into-every-source-file

static void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window::Width = width;
    Window::Height = height;
}

static void OnScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    Input->UpdateScroll((float)xOffset, (float)yOffset);
}

static void InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Window::Handle = glfwCreateWindow(Window::InitialWidth, Window::InitialHeight, Window::Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(Window::Handle);
    glfwSetFramebufferSizeCallback(Window::Handle, Resize);
    glfwSetInputMode(Window::Handle, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(Window::Handle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetScrollCallback(Window::Handle, OnScroll);
    glfwSwapInterval(1);  // TODO: fix properly
}

static void InitGL()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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

    // Stb image
    stbi_set_flip_vertically_on_load(true);
}

static void RunWindow()
{
    // TODO: remove this, just make it a single call with no if check
    if (Window::StartFullScreen)
        glfwMaximizeWindow(Window::Handle);

    while (!glfwWindowShouldClose(Window::Handle))
    {
        Time::WallTime = (float)glfwGetTime();

        Update();
        Render();

        Time::DeltaTime = (float)glfwGetTime() - Time::WallTime;
        Time::FrameRate = 1.0f / Time::DeltaTime;
        Time::UpdateCount++;

        glfwSwapBuffers(Window::Handle);
        glfwPollEvents();
    }
}

static void ShutdownGLFW()
{
    glfwTerminate();
}

int main()
{
    InitGLFW();
    InitGL();

    Initialize();
    RunWindow();
    Shutdown();

    ShutdownGLFW();
}
