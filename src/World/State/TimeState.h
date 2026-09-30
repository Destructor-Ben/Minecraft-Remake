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
        static constexpr float Noon = DayLength / 4.0f;
        static constexpr float Dusk = DayLength / 2.0f;
        static constexpr float Midnight = DayLength * 3.0f / 4.0f;
    };
}
