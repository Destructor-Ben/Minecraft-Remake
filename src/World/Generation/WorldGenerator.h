#pragma once

#include "Random/NoiseGenerator.h"

// TODO: make chunk remeshing done in the chunk renderer
// TODO: finish world generation
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
        void GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight);

        // Generates an individual chunk, used above
        void Generate(Chunk& chunk);

    private:
        void AddChunkIfExists(set<Chunk*>& chunks, vec3i chunkPos);

        // These are the passes of the world generator
        // Figure out what biomes will be where
        void GenerateBiomes(Chunk& chunk);
        // Generate the terrain according to each biome
        void GenerateTerrain(Chunk& chunk);
        // Generate caves
        void GenerateCaves(Chunk& chunk);
        // Generate large structures
        void GenerateStructures(Chunk& chunk);
        // Generate decorations, such as grass, trees, etc.
        void GenerateDecorations(Chunk& chunk);

        World* m_World = nullptr;
        uint m_Seed = 0;
        NoiseGenerator m_Noise;
    };
}
