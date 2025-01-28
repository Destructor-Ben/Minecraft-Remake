#pragma once

#include "World/Biome.h"

namespace Minecraft::Biomes
{
    // TODO: for biomes, blocks, and all other data, make a CreateBiome function that returns a raw ptr and stores the smart one in a vector
    // TODO: make a proper biome list
    inline unique_ptr<Biome> None = make_unique<Biome>();

    // Temperature
    inline unique_ptr<Biome> Grassland = make_unique<Biome>();
    inline unique_ptr<Biome> Forest = make_unique<Biome>();

    // Hot
    inline unique_ptr<Biome> Desert = make_unique<Biome>();
    inline unique_ptr<Biome> Jungle = make_unique<Biome>();

    // Cold
    inline unique_ptr<Biome> Tundra = make_unique<Biome>();
    inline unique_ptr<Biome> SnowyForest = make_unique<Biome>();
}
