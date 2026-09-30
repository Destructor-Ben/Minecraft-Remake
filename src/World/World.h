#pragma once

#include "Hash.h"
#include "Graphics/Camera.h"
#include "World/Chunk.h"
#include "World/Coords.h"
#include "World/Generation/WorldGenerator.h"

#include "World/State/PlayerState.h"
#include "World/State/SkyState.h"
#include "World/State/TimeState.h"

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
    // hello you. this is a friend of the creator, dont try to understand this mess. Have a great day (:

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

        PlayerState Player;
        SkyState Sky;
        TimeState Time;

        // TODO: move below data to a Dimension class
        // Chunk data
        unordered_map <ChunkPos, Chunk> Chunks = { };
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

        // TODO: make this done automatically in the chunk state (though maybe not, since priority is needed? at least don't allow writes without a priority or calling this)
        void OnBlockModified(const BlockPos &pos);

    private:
        void UpdateChunkList(vector<Chunk*>& chunks, int radius);

        void UpdateMeshInDirection(const ChunkPos& chunkPos, vec3i dir);

        // TODO: these could be arrays maybe instead of vectors? their size will be fixed unless the render distance changes
        // - Or just preallocate the size of the vectors, use emplace_back - rewatch cherno video on proper use of vectors
        vector<Chunk*> m_LoadedChunks = { };
        vector<Chunk*> m_RenderedChunks = { };

        ulong m_Seed = 0;
        WorldGenerator m_WorldGenerator;
    };
}
