#include "Common.h"
#include "Game.h"

namespace Minecraft
{
    GLFWwindow* window;
    uint vBuffer;
    uint iBuffer;

    void Initialize()
    {
        float vertex[] = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
        };
        glGenBuffers(1, &vBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertex, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vBuffer);

        uint indicies[] =  {0, 1, 2};
        glGenBuffers(1, &iBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 3, indicies, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    }

    void Shutdown()
    {

    }

    void Tick(float deltaTime)
    {

    }

    void Update(float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void Render()
    {
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    void OnResize(int width, int height)
    {

    }
}
