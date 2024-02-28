#include "Game.h"

#include "Common.h"

namespace Minecraft
{
    InputManager* Input = nullptr;
    Renderer* Camera = nullptr;

    static IndexBuffer* indexBuffer;
    static VertexBuffer* vertexBuffer;
    static VertexArray* vertexArray;
    static Shader* shader;

    void Initialize()
    {
        Input = new InputManager();
        Camera = new Renderer();

        Input->SetCursorDisabled(false);

        uint index[] = {
            0, 2, 1,
            1, 2, 3,
        };

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

        shader = new Shader(Shader::FromFile("res/shaders/shader"));

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, 6);

        // Vertex buffer and array
        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 3);
        vertexArray->AddBuffer(*vertexBuffer);

        Renderer::UnbindAll();
    }

    void Shutdown()
    {
        Renderer::UnbindAll();

        delete Input;
        delete Camera;

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
    }

    void Render()
    {
        Renderer::Clear();

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
