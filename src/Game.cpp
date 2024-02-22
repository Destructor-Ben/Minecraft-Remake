#include "Common.h"
#include "Game.h"

#include "Input/Input.h."
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Shader.h"

// TODO: Time - ticks count, render count, framreate, tickrate, etc. real world time?
namespace Minecraft
{
    GLFWwindow* Window;
    int ScreenWidth = InitialWidth;
    int ScreenHeight = InitialHeight;

    IndexBuffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    Shader* shader;
    uint VAO;

    void Initialize()
    {
        InitializeInput();

        float vertex[] = {
            0.0f, 0.5f,
            0.5f, 0.5f,
            0.5f, 0.0f,
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, 3, 2);
        vertexBuffer->Bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

        shader = Shader::FromFile("res/shaders/shader");
        shader->Bind();

        uint index[] = {
            0,
            1,
            2,
        };

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, 4);
        indexBuffer->Bind();
    }

    void Shutdown()
    {
        ShutdownInput();

        delete indexBuffer;
        delete vertexBuffer;
        delete shader;
        glDeleteVertexArrays(1, &VAO);
    }

    void Tick(float deltaTime)
    {

    }

    void Update(float deltaTime)
    {
        UpdateInput();

        if (WasKeyReleased(Key::Space))
            glfwSetWindowShouldClose(Window, true);
    }

    void Render()
    {
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OnResize(int width, int height)
    {
        ScreenWidth = width;
        ScreenHeight = height;
    }
}
