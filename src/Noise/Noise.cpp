#include "Noise.h"

namespace Minecraft {
    double Noise2D(uint32 seed, double x, double y) {
        int n = x + y * 57 + seed;
        n = ( n << 13 ) ^ n;
        float rand = 1 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f;
        return rand;
    }
}