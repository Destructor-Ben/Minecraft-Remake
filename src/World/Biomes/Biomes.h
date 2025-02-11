#pragma once

#include "World/BiomeType.h"

namespace Minecraft::Biomes
{
    inline int BiomeCount = 0;
    inline vector<shared_ptr<BiomeType>> BiomeList = { };

    template<typename... Params>
    inline BiomeType* CreateBiome(Params... params)
    {
        auto biome = make_shared<BiomeType>(params...);
        BiomeList.push_back(biome);
        BiomeCount++;
        return biome.get();
    }

    inline BiomeType* None = CreateBiome("None", vec3(1, 0, 1));

    // Temperature
    inline BiomeType* Grassland = CreateBiome("Grassland", vec3(0, 1, 0));
    inline BiomeType* Forest = CreateBiome("Forest", vec3(0, 0.5, 0));

    // Hot
    inline BiomeType* Desert = CreateBiome("Desert", vec3(1, 1, 0));
    inline BiomeType* Jungle = CreateBiome("Jungle", vec3(0.5, 0.5, 0));

    // Cold
    inline BiomeType* Tundra = CreateBiome("Tundra", vec3(1, 1, 1));
    inline BiomeType* SnowyForest = CreateBiome("Snowy Forest", vec3(0.5, 0.5, 0.5));
}
