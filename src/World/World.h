#pragma once

#include "../Common.h"

#include <array>
#include <unordered_map>

#include "Chunk.h"
#include "WorldGenerator.h"
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

        vector<Chunk> Chunks;
        WorldGenerator* WorldGenerator;

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
    };
}
