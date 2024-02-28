#include "Common.h"

#include "Game.h"
#include "Time.h"
#include "Graphics/Window.h"

using namespace Minecraft;

// TODO: improve error checking at some point and also make logging better
// TODO: tick thread
// TODO: Time - ticks count, render count, framreate, tickrate, etc. real world time?
// TODO: upper level abstractions for graphics stuff:
// Mesh - contains a VAO and multiple materials with index buffers for each material
// Material - a shader and binding functions
// Texture

static void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Window::Width = width;
    Window::Height = height;
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
}

static void InitGLAD()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

static void RunWindow()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, Window::InitialWidth, Window::InitialHeight);

    if (Window::StartFullScreen)
        glfwMaximizeWindow(Window::Handle);

    while (!glfwWindowShouldClose(Window::Handle))
    {
        Time::WallTime = (float)glfwGetTime();

        Update();
        Render();

        Time::DeltaTime = (float)glfwGetTime() - Time::WallTime;
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
    InitGLAD();

    Initialize();
    RunWindow();
    Shutdown();

    ShutdownGLFW();
}
