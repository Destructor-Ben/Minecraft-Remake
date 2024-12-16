#pragma once

#include "Random/NoiseGenerator.h"

namespace Minecraft
{
    class Block;
    class Chunk;
    class World;

    class WorldGenerator
    {
    public:
        WorldGenerator() = default;
        explicit WorldGenerator(World* world, uint seed = 0);

        // Generates the initial chunks in a world
        void Generate(int spawnRadius, int minHeight, int maxHeight);

        // Makes the world infinite by generating chunks around the player
        void GenerateChunksAroundPlayer(vec3 playerPos, int radius);

        // Generates an individual chunk, used above
        // Generation is composed of passes, which run sequentially
        // TODO: make generation passes
        // - Biomes
        // - Terrain
        void Generate(Chunk& chunk);

    private:
        void AddChunkIfExists(set<Chunk*>& chunks, vec3i chunkPos);

        // Uses noise functions to generate height at single block
        double GenerateHeight(float x, float z);

        World* m_World = nullptr;
        uint m_Seed = 0;
        NoiseGenerator m_Noise;
    };
}
