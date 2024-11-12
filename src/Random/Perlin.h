#pragma once

namespace Minecraft
{
    double Perlin2D(uint seed, double x, double y, double persistence, double octaveCount);
    double SmoothNoise(uint seed, double x, double y);
    double InterpolatedNoise(uint seed, double x, double y);
}
