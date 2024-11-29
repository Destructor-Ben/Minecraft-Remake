#pragma once

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
        void Generate();

        // Makes the world infinite by generating chunks around the player
        void GenerateChunksAroundPlayer(vec3 playerPos);

        // Generates an individual chunk, used above
        // Generation is composed of passes, which run sequentially
        // TODO: make generation passes
        // - Biomes
        // - Terrain
        void Generate(Chunk& chunk);

    private:
        // Uses noise functions to generate height at single block
        double GenerateHeight(float verticalScale, float horizontalScale, uint seed, int x, int z, double persistence, double octaveCount);

        World* m_World = nullptr;
        uint m_Seed = 0;
    };
}
