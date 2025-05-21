#include "Color.h"

namespace Minecraft
{
    // TODO: implement all of these
    string Color::ToHex() const
    {
        return "";
    }

    vec3 Color::ToHSV() const
    {
        return vec3();
    }

    vec4 Color::ToRGBA() const
    {
        return vec4(RGB, Opacity);
    }

    Color Color::FromHex(string hex)
    {
        return Color(vec3());
    }

    Color Color::FromHSV(vec3 hsv, float opacity)
    {
        return Color(vec3());
    }
}
