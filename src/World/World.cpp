#include "World.h"

#include "../Game.h"

namespace Minecraft
{
    World::World()
    {
        // Input
        InputManager::SetRawMouseMotion(true);
        InputManager::SetCursorDisabled(true);

        // Camera
        Camera = Minecraft::Camera();
        Camera.FOV = 70.0f;
        Camera.Position.y = 5.0f;
        Renderer->SetCamera(&Camera);

        // Chunks
        Chunk = new class Chunk(0, 0, 0);

        // Generate world
        WorldGenerator = Minecraft::WorldGenerator(*this, 0);// TODO: random seed generation
        WorldGenerator.Generate();

#pragma region Cube
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

        float v = 0.5f;
        float vertex[] = {
            -v, -v, -v,
            -v, -v, v,
            v, -v, -v,
            v, -v, v,
            -v, v, -v,
            -v, v, v,
            v, v, -v,
            v, v, v,
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
#pragma endregion
    }

    World::~World()
    {
        // Input
        InputManager::SetRawMouseMotion(false);
        InputManager::SetCursorDisabled(false);

        // Camera
        Renderer->SetCamera(nullptr);

        // Chunks
        delete Chunk;

        // Cube
        delete shader;
        delete material;
        delete indexBuffer;

        delete vertexBuffer;
        delete vertexArray;

        delete mesh;
    }

    void World::Tick()
    {
        Chunk->Tick();
    }

    void World::Update()
    {
        if (Input->WasKeyReleased(Key::Escape))
            Window::Close();

        Chunk->Update();
        UpdateCamera();
    }

    void World::Render()
    {
        Chunk->Render();
    }

    void World::UpdateCamera()
    {
        const float cameraSpeed = 15.0f;
        const float sensitivity = 0.1f;
        const float maxAngle = 89.0f;
        float speed = cameraSpeed * Time::DeltaTime;

        // Rotation
        Camera.Rotation.x -= Input->GetMousePosDelta().y * sensitivity;
        Camera.Rotation.y += Input->GetMousePosDelta().x * sensitivity;
        Camera.Rotation.z = 0;

        // Clamping
        float& xRotation = Camera.Rotation.x;
        xRotation = glm::clamp(xRotation, -maxAngle, maxAngle);

        // Input
        vec3 movementDirection = vec3(0.0f);

        if (Input->IsKeyDown(Key::W))
            movementDirection.z -= 1;

        if (Input->IsKeyDown(Key::S))
            movementDirection.z += 1;

        if (Input->IsKeyDown(Key::A))
            movementDirection.x -= 1;

        if (Input->IsKeyDown(Key::D))
            movementDirection.x += 1;

        if (Input->IsKeyDown(Key::Space))
            movementDirection.y += 1;

        if (Input->IsKeyDown(Key::LeftShift))
            movementDirection.y -= 1;

        // Vertical movement
        Camera.Position.y += movementDirection.y * speed;

        // Horizontal movement
        if (movementDirection.x != 0 || movementDirection.z != 0)
        {
            // Normalising horizontal movement direction
            vec2 horizontalDirection = glm::normalize(vec2(movementDirection.x, movementDirection.z));
            movementDirection.x = horizontalDirection.x;
            movementDirection.z = horizontalDirection.y;

            // Calculating forward and right vectors
            vec3 cameraForward = Camera.GetForwardVector();
            vec3 cameraRight = Camera.GetRightVector();

            // Disable movement on the Y axis from WASD movement
            cameraForward.y = 0.0f;
            cameraForward = glm::normalize(cameraForward);

            // Moving camera
            Camera.Position += cameraForward * -movementDirection.z * speed; // There is a negative sign here because movement direction -z is forward, but camera forward -z backwards
            Camera.Position += cameraRight * movementDirection.x * speed;
        }
    }
}
