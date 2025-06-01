#pragma once

namespace Minecraft
{
    struct Rectangle
    {
        int x = 0;
        int y = 0;
        int Width = 0;
        int Height = 0;

        Rectangle() = default;
        explicit Rectangle(int width, int height) : Width(width), Height(height) { }
        explicit Rectangle(int x, int y, int width, int height) : Width(width), Height(height), x(x), y(y) { }
        explicit Rectangle(vec2i size) : Width(size.x), Height(size.y) { }
        explicit Rectangle(vec2i pos, vec2i size) : Width(size.x), Height(size.y), x(pos.x), y(pos.y) { }

        vec2i GetSize() const { return { Width, Height }; }
        vec2i GetPosition() const { return { x, y }; }

        vec2i GetBottomLeft() const { return { x, y }; }
        vec2i GetBottomRight() const { return { x + Width, y }; }
        vec2i GetTopLeft() const { return { x, y + Height }; }
        vec2i GetTopRight() const { return { x + Width, y + Height }; }

        bool ContainsPoint(vec2i point);

        string to_string() const
        {
            return format("Rectangle(X={}, Y={}, W={}, H={})", x, y, Width, Height);
        }
    };

    inline std::ostream& operator <<(std::ostream& os, const Rectangle& rect)
    {
        os << rect.to_string();
        return os;
    }

    inline string to_string(const Rectangle& rect)
    {
        return rect.to_string();
    }
}
