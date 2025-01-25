#include "WorldGenerator.h"

#include "World/World.h"
#include "World/BlockData.h"

// TODO: make a macro for looping over all blocks in a chunk + also a chunk range loop for stuff like render distance
namespace Minecraft
{
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
        // TODO: weird issue where the input position can't be a whole number?
        const float TemperatureMapScale = 1.0f / 100.0f;
        const float MoistureMapScale = 1.0f / 100.0f;
        const float HeightMapScale = 1.0f / 100.0f;
        const float CrazyMapScale = 1.0f / 100.0f;

        float temperature = m_Noise.Fractal3D(block.GetWorldPos() * TemperatureMapScale);
        float moisture = m_Noise.Fractal3D(block.GetWorldPos() * MoistureMapScale + vec3(73487, 8456, 3247));
        float height = m_Noise.Fractal3D(block.GetWorldPos() * MoistureMapScale + vec3(73487, 8456, 3247));
        float craziness = m_Noise.Fractal3D(block.GetWorldPos() * MoistureMapScale + vec3(73487, 8456, 3247));

        return temperature >= 0.5f && moisture >= 0.5f ? Biomes::Forest.get() : Biomes::Grassland.get();
    }
}
