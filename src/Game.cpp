#include "Common.h"
#include "Game.h"

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Shader.h"

namespace Minecraft
{
    GLFWwindow* window;
    IndexBuffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    Shader* shader;

    void Initialize()
    {
        float vertex[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };

        // TODO: i believe opengl uses anti clockwise winding order for indicies
        uint index[] = {
            0,
            2,
            1
        };

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, 3);
        indexBuffer->Bind();

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, 3, 2);
        vertexBuffer->Bind();

        shader = new Shader("res/shaders/shader");
        shader->Bind();
    }

    void Shutdown()
    {
        delete indexBuffer;
        delete vertexBuffer;
        delete shader;
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
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OnResize(int width, int height)
    {

    }
}
