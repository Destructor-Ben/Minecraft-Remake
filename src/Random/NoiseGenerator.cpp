#include "NoiseGenerator.h"

#include "Hash.h"

namespace Minecraft
{
    #pragma region White Noise

    float NoiseGenerator::White1D(float x)
    {
        return HashToFloat(Hash(x));
    }

    float NoiseGenerator::White2D(vec2 v)
    {
        return White2D(v.x, v.y);
    }

    float NoiseGenerator::White2D(float x, float y)
    {
        return HashToFloat(HashMultiple(x, y));
    }

    float NoiseGenerator::White3D(vec3 v)
    {
        return White3D(v.x, v.y, v.z);
    }

    float NoiseGenerator::White3D(float x, float y, float z)
    {
        return HashToFloat(HashMultiple(x, y, z));
    }

    float NoiseGenerator::HashToFloat(ulong value)
    {
        return (float)value / (float)ULONG_LONG_MAX;
    }

    #pragma endregion

    #pragma region Perlin Noise

    float NoiseGenerator::Perlin1D(float x)
    {
        return 0;
    }

    float NoiseGenerator::Perlin2D(vec2 x)
    {
        return 0;
    }

    float NoiseGenerator::Perlin3D(vec3 x)
    {
        return 0;
    }

    #pragma endregion
}
