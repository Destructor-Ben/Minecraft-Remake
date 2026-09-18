#include "Color.h"

#include "Logger.h"

namespace Minecraft
{
    string Color::ToHex(bool hex8) const
    {
        int r = (int)(std::clamp(RGB.r * 255.0f, 0.0f, 255.0f));
        int g = (int)(std::clamp(RGB.g * 255.0f, 0.0f, 255.0f));
        int b = (int)(std::clamp(RGB.b * 255.0f, 0.0f, 255.0f));

        std::stringstream ss;
        ss << "#" << std::hex << std::setw(2) << std::setfill('0') << r
           << std::setw(2) << std::setfill('0') << g
           << std::setw(2) << std::setfill('0') << b;

        if (hex8)
        {
            int a = (int)(std::clamp(Opacity * 255.0f, 0.0f, 255.0f));
            ss << std::setw(2) << std::setfill('0') << a;
        }

        return ss.str();
    }

    vec3 Color::ToHSV() const
    {
        float r = std::clamp(RGB.r, 0.0f, 1.0f);
        float g = std::clamp(RGB.g, 0.0f, 1.0f);
        float b = std::clamp(RGB.b, 0.0f, 1.0f);

        float max = std::max({ r, g, b });
        float min = std::min({ r, g, b });
        float delta = max - min;

        float h = 0.0f;
        float s = 0.0f;
        float v = max;

        if (delta > 0.0f)
        {
            if (max == r)
            {
                h = 60.0f * ((g - b) / delta);
            }
            else if (max == g)
            {
                h = 60.0f * ((b - r) / delta + 2.0f);
            }
            else if (max == b)
            {
                h = 60.0f * ((r - g) / delta + 4.0f);
            }
            s = (max > 0.0f) ? (delta / max) : 0.0f;
        }

        if (h < 0.0f)
            h += 360.0f;

        return vec3(h, s, v);
    }

    vec4 Color::ToRGBA() const
    {
        return vec4(RGB, Opacity);
    }

    optional<Color> Color::FromHex(string hex)
    {
        if (hex.empty())
            return nullopt;

        if (hex[0] == '#')
            hex = hex.substr(1);

        // Convert 3 char codes to 6 char codes
        if (hex.length() == 3)
        {
            char r = hex[0];
            char g = hex[1];
            char b = hex[2];

            hex = format("{0}{0}{1}{1}{2}{2}", r, g, b);
        }

        if (hex.length() != 6 && hex.length() != 8)
            return nullopt;

        // Parse hex to integers
        std::stringstream ss(hex);
        unsigned int value;
        ss >> std::hex >> value;

        float r;
        float g;
        float b;
        float a = 1.0f;

        if (hex.length() == 6)
        {
            r = ((value >> 16) & 0xFF) / 255.0f;
            g = ((value >> 8) & 0xFF) / 255.0f;
            b = (value & 0xFF) / 255.0f;
        }
        else
        {
            r = ((value >> 24) & 0xFF) / 255.0f;
            g = ((value >> 16) & 0xFF) / 255.0f;
            b = ((value >> 8) & 0xFF) / 255.0f;
            a = (value & 0xFF) / 255.0f;
        }

        return Color(r, g, b, a);
    }

    Color Color::FromHSV(vec3 hsv, float opacity)
    {
        float h = std::fmod(hsv.x, 360.0f);
        float s = std::clamp(hsv.y, 0.0f, 1.0f);
        float v = std::clamp(hsv.z, 0.0f, 1.0f);

        float c = v * s;
        float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
        float m = v - c;

        float r;
        float g;
        float b;

        if (h < 60.0f)
        {
            r = c;
            g = x;
            b = 0.0f;
        }
        else if (h < 120.0f)
        {
            r = x;
            g = c;
            b = 0.0f;
        }
        else if (h < 180.0f)
        {
            r = 0.0f;
            g = c;
            b = x;
        }
        else if (h < 240.0f)
        {
            r = 0.0f;
            g = x;
            b = c;
        }
        else if (h < 300.0f)
        {
            r = x;
            g = 0.0f;
            b = c;
        }
        else
        {
            r = c;
            g = 0.0f;
            b = x;
        }

        return Color(r + m, g + m, b + m, opacity);
    }
}
