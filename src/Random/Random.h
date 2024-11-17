#pragma once

namespace Minecraft
{
    class Random
    {
    public:
        ulong Seed;

        Random(ulong seed) : Seed(seed) { }

        template<class T>
        T NextNumber(T min = 0, T max = 1);

        // Modifies the seed
        uint NextSeed();

        ulong GenerateRandomNumber();
    };
}
