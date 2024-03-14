#include "World.h"

#include "../Game.h"

namespace Minecraft
{
    void World::OnEnter()
    {
        InputManager::SetRawMouseMotion(true);
        InputManager::SetCursorDisabled(true);

        Renderer->SetCamera(&Camera);
    }

    void World::OnExit()
    {
        InputManager::SetRawMouseMotion(false);
        InputManager::SetCursorDisabled(false);

        Renderer->SetCamera(nullptr);
    }

    void World::Tick()
    {

    }

    void World::Update()
    {
        if (Input->WasKeyReleased(Key::Escape))
            Window::Close();

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

        if (Input->IsKeyDown(Key::LeftShift))
            movementDirection.y -= 1;

        if (movementDirection != glm::vec3(0.0f))
            Camera.Position += glm::normalize(movementDirection) * Time::DeltaTime * cameraSpeed;

        // Rotation - TODO
    }
}
