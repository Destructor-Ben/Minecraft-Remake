#include "Random.h"

namespace Minecraft
{
    ulong Random::NextSeed()
    {
        const ulong a = 6364136223846793005ULL; // Multiplier
        const ulong c = 1442695040888963407ULL; // Increment
        Seed = Seed * a + c; // LCG update

        // Recommended by ChatGPT, I'm not a hash expert, but it makes the distributions closer to their expected values
        ulong x = Seed;
        x ^= x >> 29;
        x *= 0x94d049bb133111ebULL;
        x ^= x >> 32;
        return x;
    }

    int Random::NextInt(int min, int max)
    {
        if (min > max)
            throw std::runtime_error("min cannot be larger than max in NextInt");

        ulong range = static_cast<ulong>(max - min + 1);
        return min + static_cast<int>(NextSeed() % range);
    }

    float Random::NextFloat(float min, float max)
    {
        if (min > max)
            throw std::runtime_error("min cannot be larger than max in NextFloat");

        // ChatGPT recommends this
        double normalized = (NextSeed() & ((1ULL << 53) - 1)) * (1.0 / (1ULL << 53));
        return (float)(min + normalized * (max - min));
    }

    bool Random::NextBool()
    {
        return NextSeed() & 1;
    }

    vec3 Random::NextPointOnSphere()
    {
        // Generate random angles
        double phi = NextFloat(0, 2 * numbers::pi);
        double cosTheta = NextFloat(-1, 1);
        double sinTheta = std::sqrt(1 - cosTheta * cosTheta);

        // Convert to Cartesian coordinates
        double x = sinTheta * std::cos(phi);
        double y = sinTheta * std::sin(phi);
        double z = cosTheta;

        return glm::normalize(vec3(x, y, z));
    }
}
