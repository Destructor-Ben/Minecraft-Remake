#include "Perlin.h"

namespace Minecraft
{
    double Perlin2D(uint32 seed, double x, double y, double Persistence, double OctaveCount)
    {
        double ret = 0;
        for (int i = 0; i < OctaveCount; i++)
        {
            double frequency = pow(2, i);
            double amplitude = pow(Persistence, i);

            ret += InterpolatedNoise(seed, x * frequency, y * frequency) * amplitude;
        }
        return ret;
    }

    // Cosine interpolation
    double InterpolatedNoise(uint32 seed, double x, double y)
    {
        int integerX = (int)x;
        int integerY = (int)y;
        double fractionalX = x - integerX;
        double fractionalY = y - integerY;

        double v1 = SmoothNoise(seed, integerX, integerY);
        double v2 = SmoothNoise(seed, integerX + 1, integerY);
        double v3 = SmoothNoise(seed, integerX, integerY + 1);
        double v4 = SmoothNoise(seed, integerX + 1, integerY + 1);

        double i1 = CosineInterpolate(v1, v2, fractionalX);
        double i2 = CosineInterpolate(v3, v4, fractionalX);

        return CosineInterpolate(i1, i2, fractionalY);
    }

    // 2D smooth noise
    double SmoothNoise(uint32 seed, double x, double y)
    {
        double corners = (IntNoise2D(seed, x + 1, y + 1) + IntNoise2D(seed, x + 1, y - 1) + IntNoise2D(seed, x - 1, y + 1) + IntNoise2D(seed, x - 1, y - 1)) / 16.0f;
        double sides = (IntNoise2D(seed, x, y + 1) + IntNoise2D(seed, x, y - 1) + IntNoise2D(seed, x - 1, y) + IntNoise2D(seed, x + 1, y)) / 8.0f;
        double center = IntNoise2D(seed, x, y) / 4.0f;
        return corners + sides + center;
    }
}
