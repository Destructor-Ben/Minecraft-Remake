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

    static glm::mat4 CameraMatrix(1.0f);

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

        CameraMatrix = glm::translate(CameraMatrix, glm::vec3(0.0f, 0.0f, -10.0f));
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

        if (Input->IsKeyDown(Key::W))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(0.0f, 0.0f, 1.0f) * 0.1f);

        if (Input->IsKeyDown(Key::S))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(0.0f, 0.0f, -1.0f) * 0.1f);

        if (Input->IsKeyDown(Key::A))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(1.0f, 0.0f, 0.0f) * 0.1f);

        if (Input->IsKeyDown(Key::D))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(-1.0f, 0.0f, 0.0f) * 0.1f);

        if (Input->IsKeyDown(Key::Space))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(0.0f, -1.0f, 0.0f) * 0.1f);

        if (Input->IsKeyDown(Key::C))
            CameraMatrix = glm::translate(CameraMatrix, glm::vec3(0.0f, 1.0f, 0.0f) * 0.1f);
    }

    void Render()
    {
        Renderer::Clear();

        uint index[] = {
                // Bottom
                0, 1, 2,
                3, 2, 1,

                // Top
                4, 6, 5,
                5, 6, 7,

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

        Shader testShader = Shader::FromFile("res/shaders/shader");
        Material testMaterial = Material(testShader);

        IndexBuffer testIndexBuffer = IndexBuffer();
        testIndexBuffer.SetData(index, sizeof(index) / sizeof(uint));

        // Vertices
        VertexBuffer testVertexBuffer = VertexBuffer();
        testVertexBuffer.SetData(vertex, sizeof(vertex));

        VertexArray textVertexArray = VertexArray();
        textVertexArray.Push(GL_FLOAT, 3);
        textVertexArray.AddBuffer(testVertexBuffer);

        // Mesh
        Mesh testMesh = Mesh(textVertexArray);
        testMesh.AddMaterial(&testMaterial, &testIndexBuffer);

        Camera->ProjectionMatrix = Camera->CreatePerspectiveMatrix();
        Camera->ViewMatrix = CameraMatrix;
        // Camera->ViewMatrix = glm::translate(Camera->ViewMatrix, glm::vec3(Window::Width / 2.0f, Window::Height / 2.0f, 0.0f)); // TODO: only needed with orthographic because perspective matrix doesnt remap NDC to window coords properly

        glm::mat4 modelMatrix(1.0f);
        // modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f)); // TODO: only needed with orthographic because perspective matrix doesnt remap NDC to window coords properly
        modelMatrix = glm::rotate(modelMatrix, Time::WallTime, glm::vec3(1.0f, 1.0f, 1.0f));

        Camera->Draw(testMesh, modelMatrix);
    }
}
