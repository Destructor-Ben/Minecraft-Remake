#include "Game.h"

#include "Common.h"

namespace Minecraft
{
    InputManager* Input = nullptr;
    Renderer* Camera = nullptr;

    static Shader* shader;
    static Material* material;
    static IndexBuffer* indexBuffer;

    static VertexBuffer* vertexBuffer;
    static VertexArray* vertexArray;

    static Mesh* mesh;

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
            0.5f, 0.5f,
            0.5f, 1.0f,
            1.0f, 0.5f,
            1.0f, 1.0f
        };

        // Material
        shader = new Shader(Shader::FromFile("res/shaders/shader"));
        material = new Material(*shader);

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, sizeof(index) / sizeof(uint));

        // Vertices
        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 2);
        vertexArray->AddBuffer(*vertexBuffer);

        // Mesh
        mesh = new Mesh(*vertexArray);
        mesh->AddMaterial(material, indexBuffer);

        Renderer::UnbindAll();
    }

    void Shutdown()
    {
        Renderer::UnbindAll();

        delete Input;
        delete Camera;

        delete shader;
        delete material;
        delete indexBuffer;

        delete vertexBuffer;
        delete vertexArray;

        delete mesh;
    }

    void Tick()
    {

    }

    void Update()
    {
        Input->Update();
    }

    void Render()
    {
        Renderer::Clear();

        Camera->ProjectionMatrix = glm::mat4(1.0f);//ortho(0.0f, (float)Window::Width, 0.0f, (float)Window::Height, 0.1f, 100.0f);
        Camera->ViewMatrix = glm::mat4(1.0f);

        Camera->Draw(*mesh, glm::mat4(1.0f));
    }
}
