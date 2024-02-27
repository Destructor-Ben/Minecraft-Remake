#include "Game.h"

#include "Input/Input.h"
#include "Graphics/Shader.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArray.h"

// TODO: Time - ticks count, render count, framreate, tickrate, etc. real world time?
// TODO: maybe make this a class instead of global variables so we can have a definite initialization order
namespace Minecraft
{
    // TODO: maybe move this to graphics class?
    GLFWwindow* Window;
    int ScreenWidth = InitialWidth;
    int ScreenHeight = InitialHeight;
    // END TODO

    InputManager Input;

    IndexBuffer* indexBuffer;
    VertexBuffer* vertexBuffer;
    VertexArray* vertexArray;
    Shader* shader;

    void Initialize()
    {
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

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 2);
        vertexArray->AddBuffer(*vertexBuffer);

        // Unbinding
        VertexArray::Unbind();
        VertexBuffer::Unbind();
        IndexBuffer::Unbind();
        Shader::Unbind();
    }

    void Shutdown()
    {
        VertexArray::Unbind();
        VertexBuffer::Unbind();
        IndexBuffer::Unbind();
        Shader::Unbind();

        delete shader;
        delete indexBuffer;
        delete vertexBuffer;
        delete vertexArray;
    }

    void Tick(float deltaTime)
    {

    }

    void Update(float deltaTime)
    {
        Input.Update();

        if (Input.WasKeyReleased(Key::Space))
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
