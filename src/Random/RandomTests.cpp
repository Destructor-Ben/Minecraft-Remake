#include "RandomTests.h"

#include "Game.h"
#include "Logger.h"
#include "Random/Random.h"

namespace Minecraft
{
    static constexpr int TestCount = 10000;
    static constexpr int RangeTestCount = 10;
    static constexpr float AcceptableError = 0.05f;

    // Occasionally there is unacceptable error for large ranges
    // That's fine though because it is expected for something like that

    static void TestBools(Random& random)
    {
        int output = 0;

        for (int i = 0; i < TestCount; ++i)
        {
            if (random.NextBool())
                output++;
        }

        float average = (float)output / (float)TestCount;
        float error = abs(average - 0.5);
        Logger::Debug(format("Bool randomness: {}, expected 0.5, error {}, count {}", average, error, TestCount));

        if (error > AcceptableError)
            Logger::Error("Unacceptable error!");
    }

    static void TestInts(Random& random)
    {
        for (int i = 0; i < RangeTestCount; ++i)
        {
            // Good enough changing the range
            int rangeMin = i;
            int rangeMax = i * 2 + 1;
            int output = 0;

            for (int j = 0; j < TestCount; ++j)
            {
                output += random.NextInt(rangeMin, rangeMax);
            }

            float average = (float)output / (float)TestCount;
            float expected = (rangeMin + rangeMax) / 2.0f;
            float error = abs(average - expected);
            Logger::Debug(format("Int randomness: {}, expected {}, error {}, count {}, range [{},{}]", average, expected, error, TestCount, rangeMin, rangeMax));

            if (error > AcceptableError)
                Logger::Error("Unacceptable error!");
        }
    }

    static void TestFloats(Random& random)
    {
        for (int i = 0; i < RangeTestCount; ++i)
        {
            float rangeMin = i * 0.98f;
            float rangeMax = i * 2.34f + 1.91f;
            float output = 0;

            for (int j = 0; j < TestCount; ++j)
            {
                output += random.NextFloat(rangeMin, rangeMax);
            }

            float average = output / (float)TestCount;
            float expected = (rangeMin + rangeMax) / 2.0f;
            float error = abs(average - expected);
            Logger::Debug(format("Float randomness: {}, expected {}, error {}, count {}, range [{},{}]", average, expected, error, TestCount, rangeMin, rangeMax));

            if (error > AcceptableError)
                Logger::Error("Unacceptable error!");
        }
    }

    static void TestRandom(Random& random)
    {
        Logger::Debug(format("[Seed {} Random Test]", random.Seed));

        TestBools(random);
        TestInts(random);
        TestFloats(random);
    }

    void RunRandomTests()
    {
        // Run multiple times on different seeds
        array<ulong, 10> seeds = {
            837462915,
            295173846,
            610285493,
            472930156,
            938471205,
            150284763,
            527194680,
            394852167,
            816429503,
            263758491,
        };

        for (int i = 0; i < seeds.size(); ++i)
        {
            // Test the random
            ulong seed = seeds[i];
            auto random = Random(seed);
            TestRandom(random);
        }
    }
}
