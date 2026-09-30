#pragma once

namespace Minecraft
{
    struct TimeState
    {
        float TimeSeconds = 0;
        float TimePercent = 0;
        bool IsDay = true;
        ulong DayCount = 0;

        static constexpr float DayLength = 60.0f * 5; // Measured in seconds
        static constexpr float Dawn = 0;
        static constexpr float Noon = 1.0f / 4.0f;
        static constexpr float Dusk = 2.0f / 4.0f;
        static constexpr float Midnight = 3.0f / 4.0f;
    };
}
