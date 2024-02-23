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

        // Shader and indices
        shader = new Shader(Shader::FromFile("res/shaders/shader"));

        uint index[] = {
            0, 2, 1,
            1, 2, 3,
        };

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, 6);

        // Vertex buffer and array
        float vertex[] = {
            -0.5f, -0.5f,
            -0.5f, 0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
        };

        vertexArray = new VertexArray();

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    }

    void Shutdown()
    {
        ShutdownInput();

        delete indexBuffer;
        delete shader;
        delete vertexBuffer;
        delete vertexArray;
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
        indexBuffer->Bind();
        shader->Bind();
        vertexArray->Bind();

        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OnResize(int width, int height)
    {
        ScreenWidth = width;
        ScreenHeight = height;
    }
}
