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

        int Calculate(int parentDimension) const
        {
            int size = Pixels + (int)(parentDimension * Percent);

            if (HasMin)
            {
                int min = MinPixels + (int)(parentDimension * MinPercent);
                if (size < min)
                    size = min;
            }

            if (HasMax)
            {
                int max = MaxPixels + (int)(parentDimension * MaxPercent);
                if (size > max)
                    size = max;
            }

            return size;
        }
    };
};
