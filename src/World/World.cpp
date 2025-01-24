#include "World.h"

#include "Game.h"
#include "LogManager.h"
#include "Input/InputManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/SkyRenderer.h"
#include "World/Chunk.h"

namespace Minecraft
{
    #pragma region Coordinate Conversion Functions

    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "ConstantParameter"

    static int FloorDivision(int x, int y)
    {
        return (x < 0) ? ((x - y + 1) / y) : (x / y);
    }

    static int Modulus(int x, int y)
    {
        return (x % y + y) % y;
    }

    #pragma clang diagnostic pop

    // TODO: Chunk::Size can't be used here for some reason, the linker whines about it

    vec3i WorldToChunkPos(vec3 pos)
    {
        return vec3i(
            FloorDivision((int)std::floor(pos.x), Chunk::Size),
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
        Instance->Graphics->SceneCamera = &PlayerCamera;
        SetMouseHidden(true);
        PlayerCamera.FOV = 70.0f;

        // TODO: random seed generation
        m_WorldGenerator = WorldGenerator(this);
        m_WorldGenerator.Generate(SpawnRadius, MinSpawnHeight, MaxSpawnHeight);
    }

    World::~World()
    {
        Instance->Graphics->SceneCamera = nullptr;
        SetMouseHidden(false);
    }

    void World::Tick()
    {
        UpdateChunkList(m_LoadedChunks, SimulationDistance);

        for (auto* chunk : GetLoadedChunks())
        {
            chunk->Tick();
        }

        TickTime();
    }

    void World::Update()
    {
        UpdateChunkList(m_RenderedChunks, RenderDistance);

        if (Instance->Input->WasKeyReleased(Key::Escape))
            Instance->Close();

        if (Instance->Input->WasKeyReleased(Key::E))
            SetMouseHidden(!IsMouseHidden());

        if (Instance->Input->WasKeyReleased(Key::B))
            Instance->ChunkGraphics->DrawChunkBorders = !Instance->ChunkGraphics->DrawChunkBorders;

        if (Instance->Input->WasKeyReleased(Key::G))
            Instance->Graphics->DrawWireframes = !Instance->Graphics->DrawWireframes;

        for (auto* chunk : GetRenderedChunks())
        {
            chunk->Update();
        }

        UpdateCamera();
        m_WorldGenerator.GenerateChunksAroundPlayer(PlayerCamera.Position, GenerationDistance, MinHeight, MaxHeight);

        Instance->SkyGraphics->Update();
    }

    void World::Render()
    {
        for (auto* chunk : GetRenderedChunks())
        {
            chunk->Render();
        }

        if (Instance->ChunkGraphics->DrawChunkBorders)
            Instance->ChunkGraphics->RenderDebugChunkBorders();

        Instance->SkyGraphics->Render();
    }

    void World::SetMouseHidden(bool hidden)
    {
        m_IsMouseHidden = hidden;
        InputManager::SetRawMouseMotion(hidden);
        InputManager::SetCursorDisabled(hidden);
    }

    optional<Chunk*> World::GetChunk(vec3i chunkPos)
    {
        if (!Chunks.contains(chunkPos))
            return nullopt;

        return &Chunks.at(chunkPos);
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
        chunks.clear();

        auto playerChunkPos = WorldToChunkPos(PlayerCamera.Position);

        for (int x = -radius + 1; x < radius; ++x)
        {
            for (int y = -radius + 1; y < radius; ++y)
            {
                for (int z = -radius + 1; z < radius; ++z)
                {
                    // Calculate chunk pos
                    auto chunkPos = vec3i(x, y, z);
                    chunkPos += playerChunkPos;

                    // Add the chunk
                    auto chunk = GetChunk(chunkPos);
                    if (!chunk.has_value())
                        continue;

                    chunks.push_back(chunk.value());
                }
            }
        }
    }

    void World::TickTime()
    {
        Time += Instance->FixedDeltaTime;

        // New days start after dawn, not midnight
        if (Time >= DayLength)
        {
            Time -= DayLength;
            DayCount++;
        }

        TimePercent = Time / DayLength;
        IsDay = Time >= Dawn && Time < Dusk;
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
        PlayerCamera.Rotation = quat(vec3(m_CameraPitch, m_CameraYaw, 0.0f));

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
        PlayerCamera.Position.y += movementDirection.y * speed;

        // Horizontal movement
        if (movementDirection.x != 0 || movementDirection.z != 0)
        {
            // Normalising horizontal movement direction
            vec2 horizontalDirection = glm::normalize(vec2(movementDirection.x, movementDirection.z));
            movementDirection.x = horizontalDirection.x;
            movementDirection.z = horizontalDirection.y;

            // Calculating forward and right vectors
            vec3 cameraForward = PlayerCamera.GetForwardVector();
            vec3 cameraRight = PlayerCamera.GetRightVector();

            // Disable movement on the Y axis from WASD movement
            cameraForward.y = 0.0f;
            cameraForward = glm::normalize(cameraForward);

            cameraRight.y = 0.0f;
            cameraRight = glm::normalize(cameraRight);

            // Moving camera
            PlayerCamera.Position += cameraForward * -movementDirection.z * speed; // There is a negative sign here because movement direction -z is forward, but camera forward -z backwards
            PlayerCamera.Position += cameraRight * movementDirection.x * speed;
        }
    }
}
