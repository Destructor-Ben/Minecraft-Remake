#pragma once

#include "Common.h"

#include "World/Chunk.h"
#include "Noise/Perlin.h"
#include "LogManager.h"

namespace Minecraft
{
    // Forward declare, just in case
    class World;

    class WorldGenerator
    {
    public:
        WorldGenerator() = default;
        explicit WorldGenerator(class World* world, uint32 seed = 0);

        // Generates the initial chunks in a world
        void Generate();

        // Generates an individual chunk, used above
        // Generation is composed of passes, which run sequentially
        // TODO: make generation passes
        // - Biomes
        // - Terrain
        void Generate(Chunk& chunk);

        // Uses noise functions to generate height at single block
        int16 GenerateHeightAtBlock(float VerticalScale, float HorizontalScale, uint32 seed, Block& block, double Persistence, double OctaveCount);

    private:
        World* m_World = nullptr;
        uint32 m_Seed = 0;
    };
}
