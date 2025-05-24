#include "UIDimension.h"

namespace Minecraft
{
    int UIDimension::Calculate(int parentDimension) const
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
}
