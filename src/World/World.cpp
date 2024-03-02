#include "World.h"

#include "../Game.h"

namespace Minecraft
{
    void World::OnEnter()
    {
    }

    void World::OnExit()
    {

    }

    void World::Tick()
    {

    }

    void World::Update()
    {
        UpdateCamera();
    }

    void World::Render()
    {

    }

    void World::UpdateCamera()
    {
        // Movement
        const float cameraSpeed = 15.0f;
        glm::vec3 movementDirection(0.0f);

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

        if (Input->IsKeyDown(Key::C))
            movementDirection.y -= 1;

        if (movementDirection != glm::vec3(0.0f))
            CameraPosition += movementDirection * Time::DeltaTime * cameraSpeed;

        // TODO: Rotation

        // Recreating matrices
        glm::mat4 viewMatrix(1.0f);
        viewMatrix = glm::translate(viewMatrix, -CameraPosition);
        Camera->ViewMatrix = viewMatrix;
        Camera->ProjectionMatrix = Camera->CreatePerspectiveMatrix();
    }
}
