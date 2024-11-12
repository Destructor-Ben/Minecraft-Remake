#include "World.h"

#include "Game.h"
#include "LogManager.h"
#include "Timers.h"
#include "Window.h"
#include "Input/InputManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"

namespace Minecraft
{
    World::World()
    {
        SetMouseHidden(true);
        Camera.FOV = 70.0f;
        Renderer->Camera = &Camera;

        // TODO: random seed generation
        Renderer->ChunkRenderer->TheWorld = this; // TODO: temporary
        m_WorldGenerator = WorldGenerator(this);
        m_WorldGenerator.Generate();
    }

    World::~World()
    {
        SetMouseHidden(false);
        Renderer->Camera = nullptr;
    }

    void World::Tick()
    {
        UpdateChunkList(m_LoadedChunks, 4);

        for (auto* chunk : GetLoadedChunks())
        {
            chunk->Tick();
        }
    }

    void World::Update()
    {
        UpdateChunkList(m_RenderedChunks, 6);

        if (Input->WasKeyReleased(Key::Escape))
            Window::Close();

        if (Input->WasKeyReleased(Key::E))
            SetMouseHidden(!IsMouseHidden());

        for (auto* chunk : GetRenderedChunks())
        {
            chunk->Update();
        }

        UpdateCamera();
        m_WorldGenerator.GenerateChunksAroundPlayer(Camera.Position);
    }

    void World::Render()
    {
        for (auto* chunk : GetRenderedChunks())
        {
            chunk->Render();
        }
    }

    void World::SetMouseHidden(bool hidden)
    {
        m_IsMouseHidden = hidden;
        InputManager::SetRawMouseMotion(hidden);
        InputManager::SetCursorDisabled(hidden);
    }

    optional<Chunk*> World::GetChunk(vec3i chunkPos)
    {
        if (Chunks.contains(chunkPos))
            return &Chunks.at(chunkPos);

        return nullopt;
    }

    optional <Block> World::GetBlock(vec3i pos)
    {
        // Calculate coordinates
        vec3i chunkPos = pos / (int)Chunk::Size;
        vec3i blockPos = pos % (int)Chunk::Size;

        // Adjust for negatives
        for (int i = 0; i < 3; ++i)
        {
            if (blockPos[i] < 0)
            {
                blockPos[i] += Chunk::Size;
                chunkPos[i]--;
            }
        }

        // Get block from chunk
        auto chunk = GetChunk(chunkPos);
        if (!chunk.has_value())
            return nullopt;

        return chunk.value()->GetBlock(blockPos);
    }

    void World::UpdateChunkList(vector<Chunk*>& chunks, int radius)
    {
        // TODO: update these properly with render distance
        chunks.clear();
        for (auto& chunk : Chunks | views::values)
        {
            chunks.push_back(&chunk);
        }
    }

    void World::UpdateCamera()
    {
        if (!m_IsMouseHidden)
            return;

        const float sensitivity = 0.005f;
        const float maxAngle = glm::radians(89.0f);
        float speed = 10.0f * Timers::DeltaTime;

        // Rotation
        m_CameraPitch -= Input->GetMousePosDelta().y * sensitivity;
        m_CameraYaw -= Input->GetMousePosDelta().x * sensitivity;
        m_CameraPitch = glm::clamp(m_CameraPitch, -maxAngle, maxAngle);
        Camera.Rotation = quat(vec3(m_CameraPitch, m_CameraYaw, 0.0f));

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

            cameraRight.y = 0.0f;
            cameraRight = glm::normalize(cameraRight);

            // Moving camera
            Camera.Position += cameraForward * -movementDirection.z * speed; // There is a negative sign here because movement direction -z is forward, but camera forward -z backwards
            Camera.Position += cameraRight * movementDirection.x * speed;
        }
    }
}
