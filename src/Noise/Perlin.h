#pragma once

#include "Noise.h"

namespace Minecraft
{
    double Perlin2D(uint32 seed, double x, double y, double persistence, double octaveCount);
    double SmoothNoise(uint32 seed, double x, double y);
    double InterpolatedNoise(uint32 seed, double x, double y);
}
