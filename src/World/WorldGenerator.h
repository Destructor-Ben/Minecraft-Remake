#pragma once

#include "src/Common.h"

#include "Chunk.h"
#include "src/Noise/Perlin.h"

namespace Minecraft
{
    // Forward declare, just in case
    class World;

    class WorldGenerator
    {
    public:
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
        World* m_World;
        uint32 m_Seed;
    };
}
