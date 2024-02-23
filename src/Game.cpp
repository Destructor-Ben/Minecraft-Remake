#include "Common.h"
#include "Game.h"

#include "Input/Input.h."
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"

// TODO: Time - ticks count, render count, framreate, tickrate, etc. real world time?
namespace Minecraft
{
    // TODO: maybe move this to graphics class?
    GLFWwindow* Window;
    int ScreenWidth = InitialWidth;
    int ScreenHeight = InitialHeight;
    // END TODO

    IndexBuffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    VertexArray* vertexArray;
    Shader* shader;

    void Initialize()
    {
        InitializeInput();

        float vertex[] = {
            -1.0f, -1.0f,
            -1.0f, 1.0f,
            0.0f, 0.0f,
        };

        vertexArray = new VertexArray();
        vertexArray->Bind();

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));
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
        indexBuffer->SetData(index, 3);
        indexBuffer->Bind();
    }

    void Shutdown()
    {
        ShutdownInput();

        delete indexBuffer;
        delete vertexBuffer;
        delete vertexArray;
        delete shader;
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
