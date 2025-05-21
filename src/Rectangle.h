#pragma once

namespace Minecraft
{
    // TODO: debug drawing for rectangles/bounds with UI
    // TODO: for all UV cases, should use this
    class Rectangle
    {
    public:
        int x = 0;
        int y = 0;
        int Width = 0;
        int Height = 0;

        Rectangle() { }
        Rectangle(int width, int height) : Width(width), Height(height) { }
        Rectangle(int x, int y, int width, int height) : Width(width), Height(height), x(x), y(y) { }
        Rectangle(vec2i size) : Width(size.x), Height(size.y) { }
        Rectangle(vec2i pos, vec2i size) : Width(size.x), Height(size.y), x(pos.x), y(pos.y) { }

        vec2i GetSize() const { return { Width, Height }; }
        vec2i GetPosition() const { return { x, y }; }

        vec2i GetBottomLeft() const { return { x, y }; }
        vec2i GetBottomRight() const { return { x + Width, y }; }
        vec2i GetTopLeft() const { return { x, y + Height }; }
        vec2i GetTopRight() const { return { x + Width, y + Height }; }

        bool ContainsPoint(vec2i point);
    };
}
