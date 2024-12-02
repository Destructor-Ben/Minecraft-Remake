#include "World.h"

#include "Game.h"
#include "LogManager.h"
#include "Input/InputManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/Chunk.h"

namespace Minecraft
{
    #pragma region Coordinate Conversion Functions

    static int FloorDivision(int x, int y)
    {
        return (x < 0) ? ((x - y + 1) / y) : (x / y);
    }

    static int Modulus(int x, int y)
    {
        return (x % y + y) % y;
    }

    vec3i WorldToChunkPos(vec3 pos)
    {
        return vec3i(
            FloorDivision((int)std::floor(pos.x), 10),
            FloorDivision((int)std::floor(pos.y), 10),
            FloorDivision((int)std::floor(pos.z), 10)
        );
    }

    vec3i WorldToBlockPos(vec3 pos)
    {
        return vec3i(
            Modulus((int)std::floor(pos.x), 10),
            Modulus((int)std::floor(pos.y), 10),
            Modulus((int)std::floor(pos.z), 10)
        );
    }

    vec3 ChunkToWorldPos(vec3i chunkPos)
    {
        return vec3(chunkPos) * 10;
    }

    vec3 BlockAndChunkToWorldPos(vec3i chunkPos, vec3i blockPos)
    {
        return ChunkToWorldPos(chunkPos) + vec3(blockPos);
    }

    #pragma endregion

    World::World()
    {
        Instance->Graphics->Camera = &Camera;
        SetMouseHidden(true);
        Camera.FOV = 70.0f;

        // TODO: random seed generation
        m_WorldGenerator = WorldGenerator(this);
        m_WorldGenerator.Generate();
    }

    World::~World()
    {
        Instance->Graphics->Camera = nullptr;
        SetMouseHidden(false);
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

        if (Instance->Input->WasKeyReleased(Key::Escape))
            Instance->Close();

        if (Instance->Input->WasKeyReleased(Key::E))
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
        vec3i chunkPos = WorldToChunkPos(pos);
        vec3i blockPos = WorldToBlockPos(pos);

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
        float speed = 10.0f * Instance->DeltaTime;

        // Rotation
        m_CameraPitch -= Instance->Input->GetMousePosDelta().y * sensitivity;
        m_CameraYaw -= Instance->Input->GetMousePosDelta().x * sensitivity;
        m_CameraPitch = glm::clamp(m_CameraPitch, -maxAngle, maxAngle);
        Camera.Rotation = quat(vec3(m_CameraPitch, m_CameraYaw, 0.0f));

        // Input
        vec3 movementDirection = vec3(0.0f);

        if (Instance->Input->IsKeyDown(Key::W))
            movementDirection.z -= 1;

        if (Instance->Input->IsKeyDown(Key::S))
            movementDirection.z += 1;

        if (Instance->Input->IsKeyDown(Key::A))
            movementDirection.x -= 1;

        if (Instance->Input->IsKeyDown(Key::D))
            movementDirection.x += 1;

        if (Instance->Input->IsKeyDown(Key::Space))
            movementDirection.y += 1;

        if (Instance->Input->IsKeyDown(Key::LeftShift))
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
