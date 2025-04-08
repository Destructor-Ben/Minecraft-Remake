#pragma once

#include "Hash.h"
#include "Graphics/Camera.h"
#include "World/Chunk.h"
#include "World/Generation/WorldGenerator.h"

#define for_block_in_chunk(blockX, blockY, blockZ, body) \
for (int blockX = 0; blockX < Chunk::Size; ++blockX) \
for (int blockY = 0; blockY < Chunk::Size; ++blockY) \
for (int blockZ = 0; blockZ < Chunk::Size; ++blockZ) \
body

#define for_block_in_chunk_2D(blockX, blockZ, body) \
for (int blockX = 0; blockX < Chunk::Size; ++blockX) \
for (int blockZ = 0; blockZ < Chunk::Size; ++blockZ) \
body

#define for_chunk_in_radius(chunkX, chunkY, chunkZ, radius, body) \
for (int chunkX = -radius + 1; chunkX < radius; ++chunkX) \
for (int chunkY = -radius + 1; chunkY < radius; ++chunkY) \
for (int chunkZ = -radius + 1; chunkZ < radius; ++chunkZ) \
body

#define for_chunk_in_radius_2D(chunkX, chunkZ, radius, body) \
for (int chunkX = -radius + 1; chunkX < radius; ++chunkX) \
for (int chunkZ = -radius + 1; chunkZ < radius; ++chunkZ) \
body

namespace Minecraft
{
    // Utility functions to convert between different coordinate systems
    vec3i WorldToChunkPos(vec3 pos);
    vec3i WorldToBlockPos(vec3 pos);

    vec3 ChunkToWorldPos(vec3i chunkPos);
    vec3 BlockAndChunkToWorldPos(vec3i chunkPos, vec3i blockPos);

    class World
    {
    public:
        // World size (vertical)
        static constexpr int MinHeight = -5;
        static constexpr int MaxHeight = 5;

        // Distances for rendering, simulating, etc.
        static constexpr int RenderDistance = 12;
        static constexpr int SimulationDistance = 12;
        static constexpr int GenerationDistance = 12;

        // Spawn size
        static constexpr int SpawnRadius = GenerationDistance;
        static constexpr int MinSpawnHeight = MinHeight;
        static constexpr int MaxSpawnHeight = MaxHeight;

        Camera PlayerCamera;
        vec3i PreviousPlayerChunkPos = { };
        unordered_map <vec3i, Chunk> Chunks = { };

        // Time
        float Time = 0;
        float TimePercent = 0;
        bool IsDay = true;
        int DayCount = 0;
        static constexpr float DayLength = 30.0f; // Measured in seconds
        static constexpr float Dawn = 0;                           // 0/4 or 4/4
        static constexpr float Noon = DayLength / 4.0f;            // 1/4
        static constexpr float Dusk = DayLength / 2.0f;            // 2/4
        static constexpr float Midnight = DayLength * 3.0f / 4.0f; // 3/4

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        vector<Chunk*> GetLoadedChunks() { return m_LoadedChunks; }
        vector<Chunk*> GetRenderedChunks() { return m_RenderedChunks; }

        optional<Chunk*> GetChunk(int chunkX, int chunkY, int chunkZ) { return GetChunk(vec3i(chunkX, chunkY, chunkZ)); }
        optional<Chunk*> GetChunk(vec3i chunkPos);

        optional <Block> GetBlock(int x, int y, int z) { return GetBlock(vec3i(x, y, z)); }
        optional <Block> GetBlock(vec3i pos);

        // Mouse hiding
        bool IsMouseHidden() const { return m_IsMouseHidden; }
        void SetMouseHidden(bool hidden);

    private:
        void UpdateChunkList(vector<Chunk*>& chunks, int radius);

        void TickTime();

        void UpdateCamera();
        void UpdateBlockBreaking();
        void UpdateMeshInDirection(vec3i chunkPos, vec3i dir);

        vector<Chunk*> m_LoadedChunks = { };
        vector<Chunk*> m_RenderedChunks = { };

        WorldGenerator m_WorldGenerator;

        float m_CameraPitch = 0.0f;
        float m_CameraYaw = 0.0f;
        bool m_IsMouseHidden = false;
    };
}
