#pragma once

#include "Hash.h"
#include "Graphics/Camera.h"
#include "World/Chunk.h"
#include "World/Coords.h"
#include "World/Generation/WorldGenerator.h"

// TODO: make these functions that accept lambdas
/*template <typename Func>
inline void for_each_block(Func&& func) {
    for (int blockX = 0; blockX < Chunk::Size; ++blockX)
        for (int blockY = 0; blockY < Chunk::Size; ++blockY)
            for (int blockZ = 0; blockZ < Chunk::Size; ++blockZ)
                func(blockX, blockY, blockZ);
}*/

// TODO: when storage order in Chunk is updated for blocks, remember to update the loops too
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
    // TODO: do a big review of this class and see if there is anything I can do to optimize it
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

        // Player
        // TODO: move to player entity
        static constexpr float PlayerReachDistance = 7.0f;
        BlockType* SelectedBlock = nullptr;
        Camera PlayerCamera;
        optional <Block> PlayerTargetBlock = nullopt;
        ChunkPos PreviousPlayerChunkPos = { };
        bool HasPlayerMovedChunks = true; // Set to true so when the world loads the chunk lists update

        // TODO: move below data to a Dimension class
        // Chunk data
        unordered_map <ChunkPos, Chunk> Chunks = { };

        // Time
        float Time = 0;
        float TimePercent = 0;
        bool IsDay = true;
        int DayCount = 0;
        static constexpr float DayLength = 90.0f; // Measured in seconds
        static constexpr float Dawn = 0;                           // 0/4 or 4/4
        static constexpr float Noon = DayLength / 4.0f;            // 1/4
        static constexpr float Dusk = DayLength / 2.0f;            // 2/4
        static constexpr float Midnight = DayLength * 3.0f / 4.0f; // 3/4

        World(ulong seed);

        void Generate();
        void OnEnter();
        void OnExit();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        // TODO: should be const references
        vector<Chunk*> GetLoadedChunks() { return m_LoadedChunks; }
        vector<Chunk*> GetRenderedChunks() { return m_RenderedChunks; }

        optional<Chunk*> GetChunk(int x, int y, int z) { return GetChunk(ChunkPos(x, y, z)); }
        optional<Chunk*> GetChunk(const ChunkPos& chunkPos);
        optional <Block> GetBlock(int x, int y, int z) { return GetBlock(BlockPos(x, y, z)); }
        optional <Block> GetBlock(const BlockPos& pos);

    private:
        void UpdateChunkList(vector<Chunk*>& chunks, int radius);

        void TickTime();

        void UpdateCamera();
        void UpdateBlockBreaking();
        void UpdateMeshInDirection(const ChunkPos& chunkPos, vec3i dir);

        // TODO: these could be arrays maybe instead of vectors? their size will be fixed unless the render distance changes
        // - Or just preallocate the size of the vectors, use emplace_back - rewatch cherno video on proper use of vectors
        vector<Chunk*> m_LoadedChunks = { };
        vector<Chunk*> m_RenderedChunks = { };

        ulong m_Seed = 0;
        WorldGenerator m_WorldGenerator;

        float m_CameraPitch = 0.0f;
        float m_CameraYaw = 0.0f;
    };
}
