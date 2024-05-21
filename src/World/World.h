#pragma once

#include "Common.h"
#include "Chunk.h"
#include "Hash.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "World/Generation/WorldGenerator.h"

namespace Minecraft
{
    class World
    {
    public:
        Camera Camera;
        unordered_map<vec3i, Chunk, ChunkKeyHash> Chunks = {};

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        vector<Chunk*> GetLoadedChunks() { return m_LoadedChunks; }
        vector<Chunk*> GetRenderedChunks() { return m_RenderedChunks; }

        Block GetBlock(int32 x, int32 y, int32 z) { return GetBlock(vec3i(x, y, z)); }
        Block GetBlock(vec3i pos);

        // Mouse hiding
        bool IsMouseHidden() const { return m_IsMouseHidden; }
        void SetMouseHidden(bool hidden);

    private:
        void UpdateChunkList(vector<Chunk*>& chunks, int32 radius);
        void UpdateCamera();

        vector<Chunk*> m_LoadedChunks = {};
        vector<Chunk*> m_RenderedChunks = {};

        WorldGenerator m_WorldGenerator;

        bool m_IsMouseHidden = false;
    };
}
