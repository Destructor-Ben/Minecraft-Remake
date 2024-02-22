#include "Common.h"
#include "Game.h"

using namespace Minecraft;

// TODO: improve error checking at some point and also make logging better
// TODO: tick thread and deltaTime
// TODO: improve window settings

static void Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    OnResize(width, height);
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
}

static void InitWindow()
{
    window = glfwCreateWindow(InitialWidth, InitialHeight, "Minecraft", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Resize);
}

static void InitGLAD()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

static void RunWindow()
{
    // TODO: somehow make this go into InitWindow
    if (StartFullscreen)
        glfwMaximizeWindow(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, InitialWidth, InitialHeight);

    while (!glfwWindowShouldClose(window))
    {
        Update(1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        Render();

        glfwSwapBuffers(window);
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
    InitWindow();
    InitGLAD();
    Initialize();

    RunWindow();

    Shutdown();
    ShutdownGLFW();
}
