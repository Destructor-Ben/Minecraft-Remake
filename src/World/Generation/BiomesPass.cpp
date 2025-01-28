#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

// TODO: make a macro for looping over all blocks in a chunk + also a chunk range loop for stuff like render distance
namespace Minecraft
{
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
        float temperature = SampleTemperatureMap(samplePos);
        float moisture = SampleMoistureMap(samplePos);

        // Calculate biome from the lookup table
        int temperatureIndex = m_BiomeMapSize - round(temperature * m_BiomeMapSize);
        int moistureIndex = m_BiomeMapSize - round(moisture * m_BiomeMapSize);

        return m_BiomeMap[temperatureIndex][moistureIndex];
    }

    // TODO: weird issue where the input position can't be a whole number?
    float WorldGenerator::SampleTemperatureMap(vec2 coords)
    {
        const float TemperatureMapScale = 1.0f / 10.0f;
        return m_Noise.Fractal2D(coords * TemperatureMapScale);
    }

    float WorldGenerator::SampleMoistureMap(vec2 coords)
    {
        const float MoistureMapScale = 1.0f / 10.0f;
        // TODO: more elegant way of handling offsets, probably just use a special seed for each map and modify the sample pos with it
        return m_Noise.Fractal2D(coords * MoistureMapScale + vec2(79487.193f, 3247123.00939f));
    }
}
