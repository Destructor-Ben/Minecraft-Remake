#pragma once

namespace Minecraft
{
    class Block;
    class Chunk;

    class WorldGenerator
    {
    public:
        WorldGenerator() = default;
        explicit WorldGenerator(class World* world, uint32 seed = 0);

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
        float64 GenerateHeight(float32 verticalScale, float32 horizontalScale, uint32 seed, int32 x, int32 z, float64 persistence, float64 octaveCount);

        class World* m_World = nullptr;
        uint32 m_Seed = 0;
    };
}
