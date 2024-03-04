#pragma once

namespace Minecraft
{
    namespace Time
    {
        extern int UpdateCount;
        extern int TickCount;

        extern float DeltaTime;
        extern float FixedDeltaTime;

        extern float FrameRate;
        extern float TickRate;

        // Seconds since the program started
        extern float WallTime;
    }
}
