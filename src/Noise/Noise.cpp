#include "Noise.h"

namespace Minecraft {
    // Hugo Elias hash function
    double IntNoise2D(uint32 seed, int32 x, int32 y) {
        int n = x + y * 57 + seed;
        n = ( n << 13 ) ^ n;
        double ret = 1 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
        return ret;
    }

    // Cosine interpolation function. Returns point x between a and b
    double CosineInterpolate(double a, double b, double x) {
        double ft = x * M_PI;
        double f = (1 - cos(ft)) * .5;
        return  a*(1-f) + b*f;
    }
}