#pragma once

namespace Minecraft
{
    namespace Timers
    {
        inline int32 UpdateCount = 0;
        inline int32 TickCount = 0;

        inline float DeltaTime = 0;
        inline float FixedDeltaTime = 0;

        inline float FrameRate = 0;
        inline float TickRate = 0;

        // Seconds since the program started - First is used in the update loop, second in the tick loop
        inline float WallTime = 0;
        inline float FixedWallTime = 0;

        inline chrono::time_point<chrono::steady_clock> StartTime;
    }
}
