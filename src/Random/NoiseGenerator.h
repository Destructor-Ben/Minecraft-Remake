#pragma once

namespace Minecraft
{
    class Texture;

    struct WhiteNoiseParams
    {
        ulong Seed = 0;
        bool Is0To1 = true;
    };

    struct PerlinNoiseParams
    {
        ulong Seed = 0;
        float Frequency = 0.5f; // Frequency can't be a whole number otherwise it aligns with the gradient grid. but we never have noise that large anyway
        bool Is0To1 = true;
    };

    struct FractalNoiseParams
    {
        ulong Seed = 0;
        int Octaves = 3;
        float Frequency = 0.5f;
        float Persistence = 0.5f; // Amplitude multiplier
        float Lacunarity = 2.0f; // Frequency multiplier
        bool Is0To1 = true;
    };

    class NoiseGenerator
    {
    public:
        NoiseGenerator() = default;
        NoiseGenerator(ulong seed);

        float White1D(float x, const WhiteNoiseParams& params) const;
        float White2D(float x, float y, const WhiteNoiseParams& params) const;
        float White3D(float x, float y, float z, const WhiteNoiseParams& params) const;
        float White2D(vec2 v, const WhiteNoiseParams& params) const { return White2D(v.x, v.y, params); }
        float White3D(vec3 v, const WhiteNoiseParams& params) const { return White3D(v.x, v.y, v.z, params); }

        // Simplex noise is far better than perlin, but I'll do it later
        float Perlin1D(float x, const PerlinNoiseParams& params) const;
        float Perlin2D(float x, float y, const PerlinNoiseParams& params) const;
        float Perlin3D(float x, float y, float z, const PerlinNoiseParams& params) const;
        float Perlin2D(vec2 v, const PerlinNoiseParams& params) const { return Perlin2D(v.x, v.y, params); }
        float Perlin3D(vec3 v, const PerlinNoiseParams& params) const { return Perlin3D(v.x, v.y, v.z, params); }

        float Fractal1D(float x, const FractalNoiseParams& params) const;
        float Fractal2D(float x, float y, const FractalNoiseParams& params) const;
        float Fractal3D(float x, float y, float z, const FractalNoiseParams& params) const;
        float Fractal2D(vec2 v, const FractalNoiseParams& params) const { return Fractal2D(v.x, v.y, params); }
        float Fractal3D(vec3 v, const FractalNoiseParams& params) const { return Fractal3D(v.x, v.y, v.z, params); }

        void SaveTexture(int width, int height, const WhiteNoiseParams& params, string path);
        void SaveTexture(int width, int height, const PerlinNoiseParams& params, string path);
        void SaveTexture(int width, int height, const FractalNoiseParams& params, string path);

    private:
        static void SaveTexture(string path, int width, int height, const vector<byte>& data);

        static ulong MurmurHash(ulong h);
        static ulong FloatToHash(float value);
        static float HashToFloat(ulong value);

        static float Fade(float t);
        static float Grad(int hash, ulong seed, float x, float y = 0.0f, float z = 0.0f);

        void CreatePermutations();

        ulong m_Seed = 0;

        // Used in the perlin algorithm, shuffled based on seed
        vector<int> m_Permutations { };
    };
}
