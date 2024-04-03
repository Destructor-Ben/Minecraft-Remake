#pragma once

#include "../Common.h"

#include "Chunk.h"

namespace Minecraft
{
    // Forward declare, just in case
    class World;

    class WorldGenerator
    {
    public:
        WorldGenerator(class World& world, uint32 seed);

        // Generates initial world with spawn chunks and some borders around so the infinite world can work nicely
        void Generate();

        // Generates an individual chunk, used above
        void Generate(int32 chunkX, int32 chunkY, int32 chunkZ);
        void Generate(Chunk& chunk);

    private:
        World* m_World;
        uint32 m_Seed;
    };
}
