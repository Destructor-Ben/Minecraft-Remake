#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

// TODO: make a macro for looping over all blocks in a chunk + also a chunk range loop for stuff like render distance
namespace Minecraft
{
    static FractalNoiseParams TemperatureMap = { 359812, 6, 0.005f };
    static FractalNoiseParams MoistureMap = { 9128461, 6, 0.01f };

    void WorldGenerator::InitBiomeMap()
    {
        // Cold
        m_BiomeMap[0][0] = Biomes::Tundra.get();
        m_BiomeMap[0][1] = Biomes::Tundra.get();
        m_BiomeMap[0][2] = Biomes::SnowyForest.get();
        m_BiomeMap[0][3] = Biomes::SnowyForest.get();

        // Temperature
        m_BiomeMap[1][0] = Biomes::Grassland.get();
        m_BiomeMap[1][1] = Biomes::Grassland.get();
        m_BiomeMap[2][0] = Biomes::Grassland.get();
        m_BiomeMap[2][1] = Biomes::Grassland.get();

        m_BiomeMap[1][2] = Biomes::Forest.get();
        m_BiomeMap[1][3] = Biomes::Forest.get();
        m_BiomeMap[2][2] = Biomes::Forest.get();
        m_BiomeMap[2][3] = Biomes::Forest.get();

        // Hot
        m_BiomeMap[3][0] = Biomes::Desert.get();
        m_BiomeMap[3][1] = Biomes::Desert.get();
        m_BiomeMap[3][2] = Biomes::Jungle.get();
        m_BiomeMap[3][3] = Biomes::Jungle.get();
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
        int temperatureIndex = std::round(temperature * m_BiomeMapSize);
        int moistureIndex = std::round(moisture * m_BiomeMapSize);

        return m_BiomeMap[temperatureIndex][moistureIndex];
    }
}
