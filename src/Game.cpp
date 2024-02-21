#include "Common.h"
#include "Game.h"

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"

namespace Minecraft
{
    GLFWwindow* window;
    IndexBuffer indexBuffer;
    VertexBuffer vertexBuffer;

    void Initialize()
    {
        float vertex[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f
        };

        // TODO: i believe opengl uses anti clockwise winding order for indicies
        uint index[] = {
            0,
            1,
            2
        };

        indexBuffer = IndexBuffer();
        indexBuffer.SetData(index, 3);
        indexBuffer.Bind();

        vertexBuffer = VertexBuffer();
        vertexBuffer.SetData(vertex, 3, 2);
        vertexBuffer.Bind();
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
        glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OnResize(int width, int height)
    {

    }
}
