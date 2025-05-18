#pragma once

namespace Minecraft::TextRenderer
{
    constexpr int CharacterSpacing = 1; // Measured in the same units as the font
    constexpr int WhitespaceSize = 3; // Same units as above
    constexpr int TextScale = 3; // Multiplier

    void Init();

    // TODO: color - Make either a color struct, or a namespace full of functions to make color easier, e.g. Color::White
    // - Will also need a Colors namespace
    // TODO: origin, scale, rotation, etc.
    void DrawText(string text, vec2i position);
    void DrawTextWithShadow(string text, vec2i position);
}
