#pragma once

#include "World/Biome.h"

namespace Minecraft::Biomes
{
    inline int BiomeCount = 0;
    inline vector<shared_ptr<Biome>> BiomeList = { };

    template<typename... Params>
    inline Biome* CreateBiome(Params... params)
    {
        auto biome = make_shared<Biome>(params...);
        BiomeList.push_back(biome);
        BiomeCount++;
        return biome.get();
    }

    inline Biome* None = CreateBiome("None", vec3(1, 0, 1));

    // Temperature
    inline Biome* Grassland = CreateBiome("Grassland", vec3(0, 1, 0));
    inline Biome* Forest = CreateBiome("Forest", vec3(0, 0.5, 0));

    // Hot
    inline Biome* Desert = CreateBiome("Desert", vec3(1, 1, 0));
    inline Biome* Jungle = CreateBiome("Jungle", vec3(0.5, 0.5, 0));

    // Cold
    inline Biome* Tundra = CreateBiome("Tundra", vec3(1, 1, 1));
    inline Biome* SnowyForest = CreateBiome("Snowy Forest", vec3(0.5, 0.5, 0.5));
}
