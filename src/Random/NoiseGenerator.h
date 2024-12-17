#pragma once

namespace Minecraft
{
    class NoiseGenerator
    {
    public:
        ulong Seed = 0;

        NoiseGenerator() = default;
        NoiseGenerator(ulong seed);

        // All go between 0 and 1
        float White1D(float x);
        float White2D(float x, float y);
        float White2D(vec2 v) { return White2D(v.x, v.y); }
        float White3D(float x, float y, float z);
        float White3D(vec3 v) { return White3D(v.x, v.y, v.z); }

        float Perlin1D(float x);
        float Perlin2D(float x, float y);
        float Perlin2D(vec2 v) { return Perlin2D(v.x, v.y); }
        float Perlin3D(float x, float y, float z);
        float Perlin2D(vec3 v) { return Perlin3D(v.x, v.y, v.z); }

        float Fractal1D(float x);
        float Fractal2D(float x, float y);
        float Fractal2D(vec2 v) { return Perlin2D(v.x, v.y); }
        float Fractal3D(float x, float y, float z);
        float Fractal2D(vec3 v) { return Perlin3D(v.x, v.y, v.z); }

    private:
        float HashToFloat(ulong value);

        float Fade(float t);
        float Lerp(float a, float b, float t);
        float Grad(int hash, float x, float y = 0.0f, float z = 0.0f);

        void CreatePermutations();

        // Used in the perlin algorithm, shuffled based on seed to change with seed
        vector<int> m_Permutations { };
    };
}
