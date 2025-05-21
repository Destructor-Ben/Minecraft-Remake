#pragma once

namespace Minecraft
{
    struct Color
    {
        vec3 RGB = vec3(1);
        float Opacity = 1.0f;

        // TODO: color list static Color Red;
        // - RGB, CMY, white, black, transparent

        // TODO: ToXX functions
        // - hex string

        // TODO: ctors/from functions
        // - hex string
        // - vec3
        // - vec3 + float
        // - 3 floats
        // - 4 floats

        // TODO: handling other colour formats, e.g. HSV
    };
}
