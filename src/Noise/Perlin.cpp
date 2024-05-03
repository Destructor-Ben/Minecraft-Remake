#include "Perlin.h"

namespace Minecraft {
    double Perlin2D(uint32 seed, double x, double y) {
        return Noise2D(seed,x,y);
    }
}