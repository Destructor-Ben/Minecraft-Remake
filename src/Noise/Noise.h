#pragma once

#include "../Common.h"

namespace Minecraft {
    double IntNoise2D(uint32 seed, int32 x, int32 y);
    double CosineInterpolate(double a, double b, double x);
}
