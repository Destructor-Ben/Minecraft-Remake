#include "Game.h"

#include "Common.h"

namespace Minecraft
{
    InputManager* Input = nullptr;
    class Renderer* Renderer = nullptr;
    class World* World = nullptr;

    static Shader* shader;
    static Material* material;
    static IndexBuffer* indexBuffer;

    static VertexBuffer* vertexBuffer;
    static VertexArray* vertexArray;

    static Mesh* mesh;

    void Initialize()
    {
        Input = new InputManager();
        Renderer = new class Renderer();
        World = new class World();
        World->OnEnter();

        uint32 index[] = {
                // Bottom
                2, 1, 0,
                1, 2, 3,

                // Top
                4, 5, 6,
                7, 6, 5,

                // Front
                1, 3, 5,
                5, 3, 7,

                // Back
                4, 2, 0,
                6, 2, 4,

                // Left
                0, 1, 4,
                4, 1, 5,

                // Right
                6, 3, 2,
                7, 3, 6,
        };

        float vertex[] = {
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
        };

        shader = new Shader(Shader::FromFile("res/shaders/shader"));
        material = new Material(*shader);

        indexBuffer = new IndexBuffer();
        indexBuffer->SetData(index, sizeof(index) / sizeof(uint32));

        // Vertices
        vertexBuffer = new VertexBuffer();
        vertexBuffer->SetData(vertex, sizeof(vertex));

        vertexArray = new VertexArray();
        vertexArray->Push(GL_FLOAT, 3);
        vertexArray->AddBuffer(*vertexBuffer);

        // Mesh
        mesh = new Mesh(*vertexArray);
        mesh->AddMaterial(material, indexBuffer);

        Renderer::UnbindAll();
    }

    void Shutdown()
    {
        World->OnExit();

        Renderer::UnbindAll();

        delete Input;
        delete Renderer;
        delete World;

        delete shader;
        delete material;
        delete indexBuffer;

        delete vertexBuffer;
        delete vertexArray;

        delete mesh;
    }

    void Tick()
    {
        World->Tick();
    }

    void Update()
    {
        Input->Update();
        World->Update();
        Renderer->Update();

        Input->PostUpdate();
    }

    void Render()
    {
        Renderer::Clear();

        World->Render();

        Renderer->Draw(*mesh);
    }
}
