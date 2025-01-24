#pragma once

#include "World/Biome.h"

namespace Minecraft::Biomes
{
    // TODO: make a proper biome list
    inline unique_ptr<Biome> None = make_unique<Biome>();
    inline unique_ptr<Biome> Grassland = make_unique<Biome>();
    inline unique_ptr<Biome> Forest = make_unique<Biome>();
}
