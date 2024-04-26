#include "World.h"

#include "../Game.h"

namespace Minecraft
{
    World::World()
    {
        // Input
        // TODO: this should be moved to a function
        InputManager::SetRawMouseMotion(true);
        InputManager::SetCursorDisabled(true);

        // Camera
        Camera.FOV = 70.0f;
        Renderer->Camera = &Camera;

        // Chunks
        // TODO: make a proper chunking system
        Chunk = new class Chunk(0, 0, 0);
        Chunk2 = new class Chunk(0, -1, 0);

        // Generate world
        WorldGenerator = new Minecraft::WorldGenerator(*this, 0);// TODO: random seed generation
        WorldGenerator->Generate();

        // TODO: move these calls into the WorldGenerator and block modification
        Chunk->RegenerateMesh();
        Chunk2->RegenerateMesh();
    }

    World::~World()
    {
        // Input
        InputManager::SetRawMouseMotion(false);
        InputManager::SetCursorDisabled(false);

        // Camera
        Renderer->Camera = nullptr;

        // Chunks
        delete Chunk;
        delete Chunk2;

        // World generator
        delete WorldGenerator;
    }

    void World::Tick()
    {
        Chunk->Tick();
        Chunk2->Tick();
    }

    void World::Update()
    {
        if (Input->WasKeyReleased(Key::Escape))
            Window::Close();

        Chunk->Update();
        Chunk2->Update();
        UpdateCamera();
    }

    void World::Render()
    {
        Chunk->Render();
        Chunk2->Render();
    }

    void World::UpdateCamera()
    {
        const float cameraSpeed = 150.0f;
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
