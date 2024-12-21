#pragma once

#include "Hash.h"
#include "Graphics/Camera.h"
#include "World/Chunk.h"
#include "World/Generation/WorldGenerator.h"

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
        static const int MinHeight = -1;
        static const int MaxHeight = 1;

        // Distances for rendering, simulating, etc.
        static const int RenderDistance = 5;
        static const int SimulationDistance = 5;
        static const int GenerationDistance = 3;

        // Spawn size
        static const int SpawnRadius = 8;
        static const int MinSpawnHeight = MinHeight;
        static const int MaxSpawnHeight = MaxHeight;

        Camera PlayerCamera;
        unordered_map <vec3i, Chunk> Chunks = { };

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
        void UpdateCamera();

        vector<Chunk*> m_LoadedChunks = { };
        vector<Chunk*> m_RenderedChunks = { };

        WorldGenerator m_WorldGenerator;

        float m_CameraPitch = 0.0f;
        float m_CameraYaw = 0.0f;
        bool m_IsMouseHidden = false;
    };
}
