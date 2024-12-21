#pragma once

namespace Minecraft
{
    // TODO: this needs rigorous testing, maybe just use the builtin random
    class Random
    {
    public:
        // Reinterpreted as the random number
        ulong Seed = 0;

        Random() = default;
        Random(ulong seed) : Seed(seed) { }

        // Modifies the seed and returns the new one
        ulong NextSeed();

        // All of these are inclusive
        int NextInt(int min = 0, int max = 1);
        float NextFloat(float min = 0.0f, float max = 1.0f);
        bool NextBool();
    };
}
