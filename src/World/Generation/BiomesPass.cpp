#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

// TODO: make a macro for looping over all blocks in a chunk + also a chunk range loop for stuff like render distance
// TODO: ability to export a biome map
namespace Minecraft
{
    static FractalNoiseParams TemperatureMap = { 359812, 6, 0.005f };
    static FractalNoiseParams MoistureMap = { 9128461, 6, 0.01f };

    void WorldGenerator::InitBiomeMap()
    {
        // Cold
        m_BiomeMap[0][0] = Biomes::Tundra;
        m_BiomeMap[0][1] = Biomes::Tundra;
        m_BiomeMap[0][2] = Biomes::SnowyForest;
        m_BiomeMap[0][3] = Biomes::SnowyForest;

        // Temperature
        m_BiomeMap[1][0] = Biomes::Grassland;
        m_BiomeMap[1][1] = Biomes::Grassland;
        m_BiomeMap[2][0] = Biomes::Grassland;
        m_BiomeMap[2][1] = Biomes::Grassland;

        m_BiomeMap[1][2] = Biomes::Forest;
        m_BiomeMap[1][3] = Biomes::Forest;
        m_BiomeMap[2][2] = Biomes::Forest;
        m_BiomeMap[2][3] = Biomes::Forest;

        // Hot
        m_BiomeMap[3][0] = Biomes::Desert;
        m_BiomeMap[3][1] = Biomes::Desert;
        m_BiomeMap[3][2] = Biomes::Jungle;
        m_BiomeMap[3][3] = Biomes::Jungle;
    }

    void WorldGenerator::GenerateBiomes(Chunk& chunk)
    {
        for (int x = 0; x < Chunk::Size; ++x)
        {
            for (int y = 0; y < Chunk::Size; ++y)
            {
                for (int z = 0; z < Chunk::Size; ++z)
                {
                    auto block = chunk.GetBlock(x, y, z);
                    block.Data.Biome = CalculateBiome(block);
                }
            }
        }
    }

    Biome* WorldGenerator::CalculateBiome(Block& block)
    {
        // Sample the maps
        vec2 samplePos = vec2(block.GetWorldPos().x, block.GetWorldPos().z);
        float temperature = m_Noise.Fractal2D(samplePos, TemperatureMap);
        float moisture = m_Noise.Fractal2D(samplePos, MoistureMap);

        // Calculate biome from the lookup table
        int temperatureIndex = ValueToBiomeIndex(temperature);
        int moistureIndex = ValueToBiomeIndex(moisture);

        return m_BiomeMap[temperatureIndex][moistureIndex];
    }

    int WorldGenerator::ValueToBiomeIndex(float value)
    {
        // Run through a sigmoid function to make biomes at the edge of the table less rare
        // Increasing k make rare biomes less rare
        const float k = 10.0f;
        value = 1.0f / (1.0f + std::exp(-k * (value - 0.5f)));

        // Lerp
        return std::round(std::lerp(0.0f, m_BiomeMapSize - 1, value));
    }
}
