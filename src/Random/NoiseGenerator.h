#pragma once

namespace Minecraft
{
    class NoiseGenerator
    {
    public:
        ulong Seed = 0;

        NoiseGenerator() = default;
        NoiseGenerator(ulong seed) : Seed(seed) { }

        // All go between 0 and 1
        float White1D(float x);
        float White2D(vec2 v);
        float White2D(float x, float y);
        float White3D(vec3 v);
        float White3D(float x, float y, float z);

        float Perlin1D(float x);
        float Perlin2D(vec2 v);
        float Perlin3D(vec3 v);

    private:
        float HashToFloat(ulong value);
    };
}
