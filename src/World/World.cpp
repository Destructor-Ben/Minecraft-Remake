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
        Camera.Position.z = 5.0f;
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
            Camera.Position += cameraForward * -movementDirection.z * speed; // TODO: is this minus sign from incorrect forward/reverse motion above? from bad view matrix?
            Camera.Position += cameraRight * movementDirection.x * speed;
        }
    }
}
