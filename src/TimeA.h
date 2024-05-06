#pragma once

// TODO: this header needs to be renamed because it clashes with std::time.h
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
