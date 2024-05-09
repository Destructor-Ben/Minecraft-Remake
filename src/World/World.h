#pragma once

#include "../Common.h"

#include <array>
#include <unordered_map>

#include "Chunk.h"
#include "World/Generation/WorldGenerator.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Camera.h"
#include "src/Graphics/Texture.h"

namespace Minecraft
{
    // TODO: make some form of static block data
    // TODO: possibly use indexing operator for accessing blocks in world and chunks
    class World
    {
    public:
        Camera Camera;

        // TODO: make a proper chunking system - Make chunk regions, a group of chunks that are dynamically loaded - that or just use a map/unordered_map of chunks
        vector<Chunk> Chunks;

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        // TODO: entity getters
        // TODO: block getters - use indexing operator

    private:
        void UpdateCamera();

        WorldGenerator m_WorldGenerator;
    };
}
