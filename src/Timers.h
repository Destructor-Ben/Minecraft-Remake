#pragma once

namespace Minecraft
{
    namespace Timers
    {
        inline int32 UpdateCount = 0;
        inline int32 TickCount = 0;

        inline float32 DeltaTime = 0;
        inline float32 FixedDeltaTime = 0;

        inline float32 FrameRate = 0;
        inline float32 TickRate = 0;

        // Seconds since the program started - First is used in the update loop, second in the tick loop
        inline float32 WallTime = 0;
        inline float32 FixedWallTime = 0;

        inline chrono::time_point<chrono::steady_clock> StartTime;
    }
}
