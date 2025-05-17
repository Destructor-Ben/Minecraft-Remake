#pragma once

namespace Minecraft
{
    // TODO: debug drawing for rectangles/bounds with UI
    // TODO: for all UV cases, should use this
    class Rectangle
    {
    public:
        int x;
        int y;
        int Width;
        int Height;

        vec2i GetSize() const { return { Width, Height }; }
        vec2i GetPosition() const { return { x, y }; }

        vec2i GetBottomLeft() const { return { x, y }; }
        vec2i GetBottomRight() const { return { x + Width, y }; }
        vec2i GetTopLeft() const { return { x, y + Height }; }
        vec2i GetTopRight() const { return { x + Width, y + Height }; }

        bool ContainsPoint(vec2i point);
    };
}
