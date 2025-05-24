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

    void DrawText(TextDrawParams text);
    vec2i GetTextSize(string text, vec2 scale = vec2(1));
}
