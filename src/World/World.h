#pragma once

#include "Chunk.h"
#include "Hash.h"
#include "Graphics/ChunkRenderer.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "World/Generation/WorldGenerator.h"

namespace Minecraft
{
    // Utility functions to convert between different coordinate systems
    // TODO: use these in other stuff
    // TODO: implement
    inline vec3i WorldPosToChunkPos(vec3 pos);
    inline vec3i WorldPosToBlockPos(vec3 pos);
    inline std::pair<vec3i, vec3i> WorldPosToChunkAndBlockPos(vec3 pos);

    inline vec3 ChunkPosToWorldPos(vec3i chunkPos);
    inline vec3 BlockPosToWorldPos(vec3i blockPos);
    inline vec3 BlockAndChunkPosToWorldPos(vec3i chunkPos, vec3i blockPos);

    class World
    {
    public:
        Camera Camera;
        unordered_map <vec3i, Chunk, ChunkKeyHash> Chunks = { };

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        vector<Chunk*> GetLoadedChunks() { return m_LoadedChunks; }
        vector<Chunk*> GetRenderedChunks() { return m_RenderedChunks; }

        optional<Chunk*> GetChunk(int32 chunkX, int32 chunkY, int32 chunkZ) { return GetChunk(vec3i(chunkX, chunkY, chunkZ)); }
        optional<Chunk*> GetChunk(vec3i chunkPos);

        optional <Block> GetBlock(int32 x, int32 y, int32 z) { return GetBlock(vec3i(x, y, z)); }
        optional <Block> GetBlock(vec3i pos);

        // Mouse hiding
        bool IsMouseHidden() const { return m_IsMouseHidden; }
        void SetMouseHidden(bool hidden);

    private:
        void UpdateChunkList(vector<Chunk*>& chunks, int32 radius);
        void UpdateCamera();

        vector<Chunk*> m_LoadedChunks = { };
        vector<Chunk*> m_RenderedChunks = { };

        WorldGenerator m_WorldGenerator;

        float m_CameraPitch = 0.0f;
        float m_CameraYaw = 0.0f;
        bool m_IsMouseHidden = false;
    };
}
