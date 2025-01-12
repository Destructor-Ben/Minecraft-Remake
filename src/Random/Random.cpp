#include "Random.h"

namespace Minecraft
{
    ulong Random::NextSeed()
    {
        const ulong a = 6364136223846793005ULL; // Multiplier
        const ulong c = 1442695040888963407ULL; // Increment
        const ulong m = ULONG_LONG_MAX;

        Seed = (a * Seed + c) % m;
        return Seed;
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

        return min + (float)NextSeed() / (float)ULONG_LONG_MAX * (max - min);
    }

    bool Random::NextBool()
    {
        return NextSeed() >> 16 & 1;
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
