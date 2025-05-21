#pragma once

namespace Minecraft
{
    struct Color
    {
        vec3 RGB = vec3(1);
        float Opacity = 1.0f;

        Color(vec4 rgba) : RGB(rgba.r, rgba.g, rgba.b), Opacity(rgba.a) { }
        Color(vec3 rgb, float opacity = 1.0f) : RGB(rgb), Opacity(opacity) { }
        Color(float r, float g, float b, float opacity = 1.0f) : RGB(r, g, b), Opacity(opacity) { }

        Color MultiplyRGB(float value) const { return Color(RGB * value, Opacity); }
        Color MultiplyRGB(vec3 value) const { return Color(RGB * value, Opacity); }
        Color MultiplyRGBA(float value) const { return Color(RGB * value, Opacity * value); }
        Color MultiplyRGBA(vec4 value) const { return Color(ToRGBA() * value); }

        string ToHex() const;
        vec3 ToHSV() const;
        vec4 ToRGBA() const;

        static Color FromHex(string hex);
        static Color FromHSV(vec3 hsv, float opacity = 1.0f);
    };
}
