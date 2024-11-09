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

        // Uses noise functions to generate height at single block
        int16 GenerateHeightAtBlock(float VerticalScale, float HorizontalScale, uint32 seed, Block& block, double Persistence, double OctaveCount);

    private:
        class World* m_World = nullptr;
        uint32 m_Seed = 0;
    };
}
