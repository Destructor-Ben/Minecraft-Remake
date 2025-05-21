#pragma once

#include "Colors.h"

namespace Minecraft
{
    struct TextDrawParams
    {
        vec2i Position = vec2i(0);
        vec2i Origin = vec2i(0);
        float Rotation = 0;
        float Depth = 0.5f;
        vec2 Scale = vec2(1);

        string Text = "";
        Color TextColor = Colors::White;
        optional<Color> ShadowColor = nullopt; // nullopt means the shadow color is calculated
        bool HasShadow = true;
    };
}
