#include "Game.h"

#include "Common.h"

#include "Input/Input.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Window.h"

namespace Minecraft
{
    InputManager* Input = nullptr;

    static IndexBuffer* indexBuffer;
    static VertexBuffer* vertexBuffer;
    static VertexArray* vertexArray;
    static Shader* shader;

    void Initialize()
    {
        Input = new InputManager();

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
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
        };

        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 3);
        vertexArray->AddBuffer(*vertexBuffer);

        // Unbinding
        VertexArray::Unbind();
        VertexBuffer::Unbind();
        IndexBuffer::Unbind();
        Shader::Unbind();
    }

    void Shutdown()
    {
        delete Input;

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
        Input->Update();

        if (Input->WasKeyReleased(Key::Space))
            Window::Close();
    }

    void Render()
    {
        indexBuffer->Bind();
        shader->Bind();
        vertexArray->Bind();

        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(90.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(10000.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection(1.0f);// = glm::perspective(glm::radians(45.0f), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);//glm::ortho(0.0f, (float)ScreenWidth, 0.0f, (float)ScreenHeight, 0.1f, 100.0f);

        shader->SetUniform("uTransform", model * view * projection);
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
