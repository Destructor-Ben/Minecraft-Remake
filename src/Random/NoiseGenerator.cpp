#include "NoiseGenerator.h"

#include <random>

#include "Hash.h"

namespace Minecraft
{
    NoiseGenerator::NoiseGenerator(ulong seed) : Seed(seed)
    {
        CreatePermutations();
    }

    #pragma region White Noise

    float NoiseGenerator::White1D(float x)
    {
        return HashToFloat(Hash(x));
    }

    float NoiseGenerator::White2D(float x, float y)
    {
        return HashToFloat(HashMultiple(x, y));
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

    // Thanks to ChatGPT for writing this code for me <3

    float NoiseGenerator::Fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float NoiseGenerator::Lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    float NoiseGenerator::Grad(int hash, float x, float y, float z)
    {
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    void NoiseGenerator::CreatePermutations()
    {
        // Initialize the base permutations
        vector<int> basePermutation(256);
        for (int i = 0; i < 256; ++i)
        {
            basePermutation[i] = i;
        }

        // Shuffle
        std::mt19937 generator(Seed);
        std::shuffle(basePermutation.begin(), basePermutation.end(), generator);

        // Duplicate the table to avoid overflow when indexing
        m_Permutations = vector<int>(512);
        for (int i = 0; i < 256; ++i)
        {
            m_Permutations[i] = basePermutation[i];
            m_Permutations[i + 256] = basePermutation[i];
        }
    }

    float NoiseGenerator::Perlin1D(float x)
    {
        int X = static_cast<int>(std::floor(x)) & 255;
        x -= std::floor(x);
        float u = Fade(x);

        int A = m_Permutations[X], B = m_Permutations[X + 1];

        return Lerp(Grad(A, x), Grad(B, x - 1), u) * 0.5f + 0.5f;
    }

    float NoiseGenerator::Perlin2D(float x, float y)
    {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);

        float u = Fade(x);
        float v = Fade(y);

        int A = m_Permutations[X] + Y;
        int B = m_Permutations[X + 1] + Y;

        return Lerp(Lerp(Grad(m_Permutations[A], x, y),
                         Grad(m_Permutations[B], x - 1, y), u),
                    Lerp(Grad(m_Permutations[A + 1], x, y - 1),
                         Grad(m_Permutations[B + 1], x - 1, y - 1), u),
                    v) * 0.5f + 0.5f;
    }

    float NoiseGenerator::Perlin3D(float x, float y, float z)
    {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);

        float u = Fade(x);
        float v = Fade(y);
        float w = Fade(z);

        int A = m_Permutations[X] + Y, AA = m_Permutations[A] + Z, AB = m_Permutations[A + 1] + Z;
        int B = m_Permutations[X + 1] + Y, BA = m_Permutations[B] + Z, BB = m_Permutations[B + 1] + Z;

        return Lerp(Lerp(Lerp(Grad(m_Permutations[AA], x, y, z),
                              Grad(m_Permutations[BA], x - 1, y, z), u),
                         Lerp(Grad(m_Permutations[AB], x, y - 1, z),
                              Grad(m_Permutations[BB], x - 1, y - 1, z), u),
                         v),
                    Lerp(Lerp(Grad(m_Permutations[AA + 1], x, y, z - 1),
                              Grad(m_Permutations[BA + 1], x - 1, y, z - 1), u),
                         Lerp(Grad(m_Permutations[AB + 1], x, y - 1, z - 1),
                              Grad(m_Permutations[BB + 1], x - 1, y - 1, z - 1), u),
                         v),
                    w) * 0.5f + 0.5f;
    }

    #pragma endregion

    #pragma region Fractal Noise

    float NoiseGenerator::Fractal1D(float x, float amplitude, float frequency, int octaves, float persistence, float lacunarity)
    {
        float noiseValue = 0.0f;
        float maxAmplitude = 0.0f; // Used to normalize to between 0 and 1

        for (int i = 0; i < octaves; ++i)
        {
            // Needs remapping to -1 to 1 so it adds properly and doesn't bias to larger values
            float perlinValue = 2.0f * Perlin1D(x * frequency) - 1.0f;

            noiseValue += amplitude * perlinValue;
            maxAmplitude += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        // Normalize the result to 0 - 1
        return (noiseValue / maxAmplitude + 1.0f) / 2.0f;
    }

    float NoiseGenerator::Fractal2D(float x, float y, float amplitude, float frequency, int octaves, float persistence, float lacunarity)
    {
        float noiseValue = 0.0f;
        float maxAmplitude = 0.0f; // Used to normalize to between 0 and 1

        for (int i = 0; i < octaves; ++i)
        {
            // Needs remapping to -1 to 1 so it adds properly and doesn't bias to larger values
            float perlinValue = 2.0f * Perlin2D(x * frequency, y * frequency) - 1.0f;

            noiseValue += amplitude * perlinValue;
            maxAmplitude += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        // Normalize the result to 0 - 1
        return (noiseValue / maxAmplitude + 1.0f) / 2.0f;
    }

    float NoiseGenerator::Fractal3D(float x, float y, float z, float amplitude, float frequency, int octaves, float persistence, float lacunarity)
    {
        float noiseValue = 0.0f;
        float maxAmplitude = 0.0f; // Used to normalize to between 0 and 1

        for (int i = 0; i < octaves; ++i)
        {
            // Needs remapping to -1 to 1 so it adds properly and doesn't bias to larger values
            float perlinValue = 2.0f * Perlin3D(x * frequency, y * frequency, z * frequency) - 1.0f;

            noiseValue += amplitude * perlinValue;
            maxAmplitude += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }

        // Normalize the result to 0 - 1
        return (noiseValue / maxAmplitude + 1.0f) / 2.0f;
    }

    #pragma endregion
}
