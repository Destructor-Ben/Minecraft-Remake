#pragma once

#include "Random/NoiseGenerator.h"

// TODO: make chunk remeshing done in the chunk renderer
// TODO: finish world generation
namespace Minecraft
{
    class Biome;
    class Block;
    class BlockType;
    class Chunk;
    class World;

    class WorldGenerator
    {
    public:
        WorldGenerator() = default;
        explicit WorldGenerator(World* world, ulong seed = 0);

        // Generates the initial chunks in a world
        void Generate(int spawnRadius, int minHeight, int maxHeight);

        // Makes the world infinite by generating chunks around the player
        void GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight);

    private:
        // Creates a chunk at the given positions and generates it
        Chunk& CreateChunk(vec3i chunkPos);

        // Generates the terrain of an individual chunk, used above
        void Generate(Chunk& chunk);

        World* m_World = nullptr;
        ulong m_Seed = 0;
        NoiseGenerator m_Noise;

        static constexpr array<vec3i, 7> m_ChunkRemeshDirections = {
            vec3i(0, 0, 0),
            vec3i(1, 0, 0),
            vec3i(-1, 0, 0),
            vec3i(0, 1, 0),
            vec3i(0, -1, 0),
            vec3i(0, 0, 1),
            vec3i(0, 0, -1),
        };

        // These are the passes of the world generator

        #pragma region Biomes

        // Figure out what biomes will be where
        void GenerateBiomes(Chunk& chunk);

        void InitBiomeMap();
        Biome* CalculateBiome(vec3 pos);
        static int ValueToBiomeIndex(float value);
        void ExportBiomeMap(int width = 1000, int height = 1000);

        static constexpr int m_BiomeMapSize = 4;
        array <array<Biome*, m_BiomeMapSize>, m_BiomeMapSize> m_BiomeMap;

        #pragma endregion

        #pragma region Terrain

        // Generate the terrain according to each biome
        void GenerateTerrain(Chunk& chunk);

        void InitSurfaceBlocksMap();

        int GetTerrainHeight(vec3 pos);
        int GetSurfaceHeight(vec3 pos);

        unordered_map<Biome*, BlockType*> m_SurfaceBlocksMap;
        unordered_map<Biome*, BlockType*> m_UndergroundBlocksMap;

        #pragma endregion

        // Generate caves
        void GenerateCaves(Chunk& chunk);

        // Generate large structures
        void GenerateStructures(Chunk& chunk);

        #pragma region Decorations

        // Generate decorations, such as grass, trees, etc.
        void GenerateDecorations(Chunk& chunk);

        void GenerateGrass(Chunk& chunk);
        bool ContainsGrass(Block& block);

        void GenerateTrees(Chunk& chunk);
        void PlaceTree(Chunk& chunk, vec3i pos, vec3i blockPos);
        bool ContainsTreeOrigin(vec3i pos);
        int GetTreeHeight(vec3i pos);

        #pragma endregion
    };
}
