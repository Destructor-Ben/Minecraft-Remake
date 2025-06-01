#include "WorldGenerator.h"

#include "Game.h"
#include "Logger.h"
#include "World/World.h"
#include "World/BlockData.h"

namespace Minecraft
{
    // Make biomes that require more extreme weather more common
    static constexpr float BiomeRarity = 15.0f;

    static FractalNoiseParams TemperatureMap = { 359812, 4, 0.00075f };
    static FractalNoiseParams MoistureMap = { 9128461, 4, 0.0025f };

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

        // Debug exports
        m_Noise.SaveTexture(1000, 1000, TemperatureMap, "TemperatureMap.png");
        m_Noise.SaveTexture(1000, 1000, MoistureMap, "MoistureMap.png");
        ExportBiomeMap();
    }

    void WorldGenerator::GenerateBiomes(Chunk& chunk)
    {
        for_block_in_chunk(x, y, z, {
            auto block = chunk.GetBlock(x, y, z);
            block.Data.Biome = CalculateBiome(block.GetWorldPos());
        })
    }

    BiomeType* WorldGenerator::CalculateBiome(vec3 pos)
    {
        // Sample the maps
        vec2 samplePos = vec2(pos.x, pos.z);
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
        const float k = BiomeRarity;
        value = 1.0f / (1.0f + std::exp(-k * (value - 0.5f)));

        // Lerp
        return std::round(std::lerp(0.0f, m_BiomeMapSize - 1, value));
    }

    void WorldGenerator::ExportBiomeMap(int width, int height)
    {
        auto data = vector<byte>();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                BiomeType* biome = CalculateBiome(vec3(x, 0, y));
                vec3 color = biome->MapColor;

                data.push_back((byte)(color.r * UCHAR_MAX));
                data.push_back((byte)(color.g * UCHAR_MAX));
                data.push_back((byte)(color.b * UCHAR_MAX));
            }
        }

        // TODO: use resource manager instead for saving textures
        string path = "BiomeMap.png";
        int channels = 3;
        int result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);

        if (!result)
            Logger::Throw("Failed to save texture at path: " + path);
    }
}
