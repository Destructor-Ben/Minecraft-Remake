#include "NoiseGenerator.h"

#include <random>

#include "Game.h"
#include "Hash.h"
#include "LogManager.h"

namespace Minecraft
{
    NoiseGenerator::NoiseGenerator(ulong seed) : m_Seed(seed)
    {
        CreatePermutations();
    }

    #pragma region White Noise

    float NoiseGenerator::White1D(float x, const WhiteNoiseParams& params) const
    {
        ulong seed = params.Seed;
        HashCombine(seed, m_Seed, FloatToHash(x));
        float value = HashToFloat(MurmurHash(seed));
        return params.Is0To1 ? value : value * 2 - 1;
    }

    float NoiseGenerator::White2D(float x, float y, const WhiteNoiseParams& params) const
    {
        ulong seed = params.Seed;
        HashCombine(seed, m_Seed, FloatToHash(x), FloatToHash(y));
        float value = HashToFloat(MurmurHash(seed));
        return params.Is0To1 ? value : value * 2 - 1;
    }

    float NoiseGenerator::White3D(float x, float y, float z, const WhiteNoiseParams& params) const
    {
        ulong seed = params.Seed;
        HashCombine(seed, m_Seed, FloatToHash(x), FloatToHash(y), FloatToHash(z));
        float value = HashToFloat(MurmurHash(seed));
        return params.Is0To1 ? value : value * 2 - 1;
    }

    ulong NoiseGenerator::MurmurHash(ulong h)
    {
        h ^= h >> 33;
        h *= 0xff51afd7ed558ccdULL;
        h ^= h >> 33;
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= h >> 33;
        return h;
    }

    ulong NoiseGenerator::FloatToHash(float value)
    {
        return (ulong)std::bit_cast<uint>(value);
    }

    float NoiseGenerator::HashToFloat(ulong value)
    {
        return (value & 0xFFFFFF) / float(0x1000000);
    }

    #pragma endregion

    #pragma region Perlin Noise

    // Thanks to ChatGPT for writing this code for me <3
    // I understand how this works now, it's actually quite simple

    float NoiseGenerator::Fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float NoiseGenerator::Grad(int hash, ulong seed, float x, float y, float z)
    {
        int h = (hash ^ (int)seed) & 15;
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
        std::mt19937 generator(m_Seed);
        std::shuffle(basePermutation.begin(), basePermutation.end(), generator);

        // Duplicate the table to avoid overflow when indexing
        m_Permutations = vector<int>(512);
        for (int i = 0; i < 256; ++i)
        {
            m_Permutations[i] = basePermutation[i];
            m_Permutations[i + 256] = basePermutation[i];
        }
    }

    float NoiseGenerator::Perlin1D(float x, const PerlinNoiseParams& params) const
    {
        x *= params.Frequency;

        int X = static_cast<int>(std::floor(x)) & 255;
        x -= std::floor(x);
        float u = Fade(x);

        int A = m_Permutations[X], B = m_Permutations[X + 1];

        float value = std::lerp(Grad(A, params.Seed, x), Grad(B, params.Seed, x - 1), u);

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    float NoiseGenerator::Perlin2D(float x, float y, const PerlinNoiseParams& params) const
    {
        x *= params.Frequency;
        y *= params.Frequency;

        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);

        float u = Fade(x);
        float v = Fade(y);

        int A = m_Permutations[X] + Y;
        int B = m_Permutations[X + 1] + Y;

        float value = std::lerp(std::lerp(Grad(m_Permutations[A], params.Seed, x, y),
                                          Grad(m_Permutations[B], params.Seed, x - 1, y), u),
                                std::lerp(Grad(m_Permutations[A + 1], params.Seed, x, y - 1),
                                          Grad(m_Permutations[B + 1], params.Seed, x - 1, y - 1), u),
                                v);

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    float NoiseGenerator::Perlin3D(float x, float y, float z, const PerlinNoiseParams& params) const
    {
        x *= params.Frequency;
        y *= params.Frequency;
        z *= params.Frequency;

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

        float value = std::lerp(std::lerp(std::lerp(Grad(m_Permutations[AA], params.Seed, x, y, z),
                                                    Grad(m_Permutations[BA], params.Seed, x - 1, y, z), u),
                                          std::lerp(Grad(m_Permutations[AB], params.Seed, x, y - 1, z),
                                                    Grad(m_Permutations[BB], params.Seed, x - 1, y - 1, z), u),
                                          v),
                                std::lerp(std::lerp(Grad(m_Permutations[AA + 1], params.Seed, x, y, z - 1),
                                                    Grad(m_Permutations[BA + 1], params.Seed, x - 1, y, z - 1), u),
                                          std::lerp(Grad(m_Permutations[AB + 1], params.Seed, x, y - 1, z - 1),
                                                    Grad(m_Permutations[BB + 1], params.Seed, x - 1, y - 1, z - 1), u),
                                          v),
                                w);

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    #pragma endregion

    #pragma region Fractal Noise

    float NoiseGenerator::Fractal1D(float x, const FractalNoiseParams& params) const
    {
        float value = 0.0f;
        float frequency = params.Frequency;
        float amplitude = 1.0f;
        float maxAmplitude = 0.0f;

        for (int i = 0; i < params.Octaves; ++i)
        {
            auto perlinParams = PerlinNoiseParams(params.Seed, frequency, false);
            value += Perlin1D(x, perlinParams) * amplitude;

            maxAmplitude += amplitude;
            amplitude *= params.Persistence;
            frequency *= params.Lacunarity;
        }

        // Remap to -1 and 1
        value /= maxAmplitude;

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    float NoiseGenerator::Fractal2D(float x, float y, const FractalNoiseParams& params) const
    {
        float value = 0.0f;
        float frequency = params.Frequency;
        float amplitude = 1.0f;
        float maxAmplitude = 0.0f;

        for (int i = 0; i < params.Octaves; ++i)
        {
            auto perlinParams = PerlinNoiseParams(params.Seed, frequency, false);
            value += Perlin2D(x, y, perlinParams) * amplitude;

            maxAmplitude += amplitude;
            amplitude *= params.Persistence;
            frequency *= params.Lacunarity;
        }

        // Remap to -1 and 1
        value /= maxAmplitude;

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    float NoiseGenerator::Fractal3D(float x, float y, float z, const FractalNoiseParams& params) const
    {
        float value = 0.0f;
        float frequency = params.Frequency;
        float amplitude = 1.0f;
        float maxAmplitude = 0.0f;

        for (int i = 0; i < params.Octaves; ++i)
        {
            auto perlinParams = PerlinNoiseParams(params.Seed, frequency, false);
            value += Perlin3D(x, y, z, perlinParams) * amplitude;

            maxAmplitude += amplitude;
            amplitude *= params.Persistence;
            frequency *= params.Lacunarity;
        }

        // Remap to -1 and 1
        value /= maxAmplitude;

        return params.Is0To1 ? value * 0.5f + 0.5f : value;
    }

    #pragma endregion

    #pragma region Texture Generation

    void NoiseGenerator::SaveTexture(int width, int height, const WhiteNoiseParams& params, string path)
    {
        auto data = vector<byte>();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                float value = White2D(x, y, params);
                if (value < 0)
                    value = 0;

                data.push_back((byte)(value * UCHAR_MAX));
            }
        }

        SaveTexture(path, width, height, data);
    }

    void NoiseGenerator::SaveTexture(int width, int height, const PerlinNoiseParams& params, string path)
    {
        auto data = vector<byte>();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                float value = Perlin2D(x, y, params);
                if (value < 0)
                    value = 0;

                data.push_back((byte)(value * UCHAR_MAX));
            }
        }

        SaveTexture(path, width, height, data);
    }

    void NoiseGenerator::SaveTexture(int width, int height, const FractalNoiseParams& params, string path)
    {
        auto data = vector<byte>();

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                float value = Fractal2D(x, y, params);
                if (value < 0)
                    value = 0;

                data.push_back((byte)(value * UCHAR_MAX));
            }
        }

        SaveTexture(path, width, height, data);
    }

    void NoiseGenerator::SaveTexture(string path, int width, int height, const vector <byte>& data)
    {
        int channels = 1;
        int result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);

        if (!result)
            Instance->Logger->Throw("Failed to save texture at path: " + path);
    }

    #pragma endregion
}
