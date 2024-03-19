#include "World.h"

#include "../Game.h"

namespace Minecraft
{
    void World::OnEnter()
    {
        InputManager::SetRawMouseMotion(true);
        InputManager::SetCursorDisabled(true);

        Renderer->SetCamera(&Camera);
        Camera.FOV = 70.0f;
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
        const float cameraSpeed = 15.0f;
        const float sensitivity = 0.1f;
        float speed = cameraSpeed * Time::DeltaTime;

        // Rotation
        // TODO: clamp x axis rotation
        Camera.Rotation.x -= Input->GetMousePosDelta().y * sensitivity;
        Camera.Rotation.y += Input->GetMousePosDelta().x * sensitivity;
        Camera.Rotation.z = 0;

        // Movement
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
        if (movementDirection.x != 0 && movementDirection.z != 0)
        {
            vec2 horizontalDirection = glm::normalize(vec2(movementDirection.x, movementDirection.z));
            float horizontalForward = horizontalDirection.y;
            float horizontalSideways = horizontalDirection.x;

            vec3 cameraRight = Camera.GetRightVector();
            vec3 cameraFront = Camera.GetForwardVector();
            cameraFront.z = 0.0f;
            cameraFront = glm::normalize(cameraFront);

            Camera.Position += cameraFront * horizontalForward * speed;
            Camera.Position += cameraRight * horizontalSideways * speed;
        }
    }
}
