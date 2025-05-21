#pragma once

#include "Graphics/TextDrawParams.h"

namespace Minecraft::TextRenderer
{
    constexpr int TextScale = 5; // Multiplier

    // These are measured in the same units as the font
    constexpr int CharacterSpacing = 1;
    constexpr int WhitespaceSize = 3;
    constexpr int ShadowOffset = 1;

    void Init();

    //void DrawText(TextDrawParams text);

    // TODO: make internal functions
    // TODO: color - Make either a color struct, or a namespace full of functions to make color easier, e.g. Color::White
    // - Will also need a Colors namespace
    // TODO: origin, scale, rotation, etc.
    void DrawText(string text, vec2i position, vec3 color = vec3(1));
    void DrawTextWithShadow(string text, vec2i position, vec3 textColor = vec3(1), optional <vec3> shadowColor = nullopt);

    // TODO: function to get size of Text
    vec2i GetTextSize(string text);
}
