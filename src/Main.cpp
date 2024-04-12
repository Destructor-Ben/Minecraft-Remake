#include "Common.h"

#include "Game.h"
#include "Time.h"
#include "Graphics/Window.h"

using namespace Minecraft;

// TODO: tick thread - https://stackoverflow.com/questions/52260084/how-to-maintain-certain-frame-rate-in-different-threads
// TODO: fix frame rate
// TODO: make the renderer take ownership of graphics resources and dispose of them automatically
// TODO: use quaternions for rotation instead of euler angles
// TODO: use proper typedefs and namespaces, as well as sorted includes
// TODO: embed resources into executable by creating a cpp and header file for each resource with their info (path, size, and bytes) - https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake
// TODO: fix random crash from glm miscalculation because the window isn't focused - also make the window behave better when not selected

// TODO: better error handling: finish callback functions, update opengl, wrap main loop in try catch to avoid exceptions from crashing the game
static void GLFWError(int32 code, cstring description)
{
    Log(format("GLFW Error {}: {}", to_string(code), description));
}

static void GLError(GLenum source,
                        GLenum type,
                        GLuint id,
                        GLenum severity,
                        GLsizei length,
                        cstring message,
                        const void* userParam)
{
    //fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
    //         ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
    //         type, severity, message );
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
    glfwInit();
    // TODO: update OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Window::Handle = glfwCreateWindow(Window::InitialWidth, Window::InitialHeight, Window::Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(Window::Handle);
    glfwSetErrorCallback(GLFWError);

    glfwSetFramebufferSizeCallback(Window::Handle, Resize);
    glfwSetInputMode(Window::Handle, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(Window::Handle, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetScrollCallback(Window::Handle, OnScroll);
    glfwSwapInterval(1); // TODO: fix properly
}

static void InitGL()
{
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // TODO: update opengl to latest version so we can have the debug output
    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(GLError, nullptr);

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
    glfwMaximizeWindow(Window::Handle);

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
