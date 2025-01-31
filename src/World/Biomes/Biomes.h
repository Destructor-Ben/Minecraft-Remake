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

    inline Biome* None = CreateBiome("None");

    // Temperature
    inline Biome* Grassland = CreateBiome("Grassland");
    inline Biome* Forest = CreateBiome("Forest");

    // Hot
    inline Biome* Desert = CreateBiome("Desert");
    inline Biome* Jungle = CreateBiome("Jungle");

    // Cold
    inline Biome* Tundra = CreateBiome("Tundra");
    inline Biome* SnowyForest = CreateBiome("Snowy Forest");
}
