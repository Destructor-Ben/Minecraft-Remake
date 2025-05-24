#pragma once

namespace Minecraft
{
    struct UIDimension
    {
        int Pixels = 0;
        float Percent = 0;

        bool HasMin = false;
        int MinPixels = 0;
        float MinPercent = 0;

        bool HasMax = false;
        int MaxPixels = 0;
        float MaxPercent = 0;

        int Calculate(int parentDimension) const;
    };
};
