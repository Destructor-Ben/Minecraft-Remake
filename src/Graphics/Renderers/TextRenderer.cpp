#include "TextRenderer.h"

#include "Game.h"
#include "Rectangle.h"
#include "ResourceManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderers/UIRenderer.h"

namespace Minecraft::TextRenderer
{
    static unordered_map<char, Rectangle> CharacterMap;
    static shared_ptr <Texture> FontTexture;
    static Rectangle UnknownCharacter = Rectangle(1, 2, 3, 5);

    int GetWhitespaceWidth(char c);

    void Init()
    {
        // Request the font
        FontTexture = Instance->Resources->RequestTexture("ui/font");

        // Init the character map
        // Letters
        for (int i = 0; i < 26; ++i)
        {
            Rectangle rect = { 1 + 4 * i, 14, 3, 5 };

            // M, N, W, and X are wider than other characters
            if (i > 12)
                rect.x += 2;

            if (i == 12)
                rect.Width += 2;

            if (i > 13)
                rect.x += 1;

            if (i == 13)
                rect.Width += 1;

            if (i > 22)
                rect.x += 2;

            if (i == 22)
                rect.Width += 2;

            if (i > 23)
                rect.x += 2;

            if (i == 23)
                rect.Width += 2;

            CharacterMap['A' + i] = rect;
            CharacterMap['a' + i] = rect;
        }

        // Numbers
        for (int i = 0; i < 10; ++i)
        {
            Rectangle rect = { 1 + 4 * i, 8, 3, 5 };
            CharacterMap['0' + i] = rect;
        }

        // Punctuation - Has to be done manually :(
        CharacterMap['.'] = { 5, 2, 1, 5 };
        CharacterMap[','] = { 7, 1, 1, 6 };
        CharacterMap['?'] = { 9, 2, 3, 5 };
        CharacterMap['!'] = { 13, 2, 1, 5 };
        CharacterMap['\''] = { 15, 2, 1, 5 };
        CharacterMap['`'] = { 17, 2, 2, 5 };
        CharacterMap['"'] = { 20, 2, 3, 5 };
        CharacterMap['-'] = { 24, 2, 3, 5 };
        CharacterMap['_'] = { 28, 2, 3, 5 };
        CharacterMap['/'] = { 32, 2, 5, 5 };
        CharacterMap['|'] = { 38, 2, 1, 5 };
        CharacterMap['\\'] = { 40, 2, 5, 5 };
        CharacterMap[':'] = { 46, 2, 1, 5 };
        CharacterMap[';'] = { 48, 1, 1, 6 };
        CharacterMap['('] = { 50, 2, 2, 5 };
        CharacterMap[')'] = { 53, 2, 2, 5 };
        CharacterMap['['] = { 56, 2, 2, 5 };
        CharacterMap[']'] = { 59, 2, 2, 5 };
        CharacterMap['{'] = { 62, 2, 3, 5 };
        CharacterMap['}'] = { 66, 2, 3, 5 };
        CharacterMap['<'] = { 70, 2, 3, 5 };
        CharacterMap['>'] = { 74, 2, 3, 5 };
        CharacterMap['#'] = { 78, 2, 5, 5 };
        CharacterMap['^'] = { 84, 2, 3, 5 };
        CharacterMap['*'] = { 88, 2, 3, 5 };
        CharacterMap['+'] = { 92, 2, 3, 5 };
        CharacterMap['='] = { 96, 2, 3, 5 };
        CharacterMap['~'] = { 100, 2, 5, 5 };
        CharacterMap['@'] = { 106, 2, 4, 5 };
        CharacterMap['$'] = { 111, 1, 5, 7 };
        CharacterMap['%'] = { 117, 2, 5, 5 };
        CharacterMap['&'] = { 123, 2, 5, 5 };
    }

    void DrawText(string text, vec2i position, vec3 color)
    {
        // Used to stop spacing at the beginning of the text
        bool previousCharWasWhitespace = true;

        for (char c : text)
        {
            // Whitespace
            int whitespaceWidth = GetWhitespaceWidth(c);
            if (whitespaceWidth != 0)
            {
                position.x += whitespaceWidth * WhitespaceSize * TextScale;
                previousCharWasWhitespace = true;
                continue;
            }

            // Character spacing - This is done here to avoid whitespaces being a pixel too large
            if (!previousCharWasWhitespace)
                position.x += CharacterSpacing * TextScale;

            // Get the character UVs
            auto charUVs = UnknownCharacter;
            if (CharacterMap.contains(c))
                charUVs = CharacterMap.at(c);

            // 3 characters have a y offset: .;$
            int yOffset = 0;
            if (c == ',' || c == ';' || c == '$')
                yOffset = -TextScale;

            // Draw the sprite
            auto sprite = Sprite();
            sprite.Position = position + vec2i(0, yOffset);
            sprite.Scale = vec2(0);
            sprite.Size = charUVs.GetSize() * TextScale;
            sprite.SpriteTexture = FontTexture;
            sprite.Color = color;
            sprite.UVs = charUVs;
            Instance->UI->DrawSprite(sprite);

            // Advance the position
            position.x += charUVs.Width * TextScale; // Character size
            previousCharWasWhitespace = false;
        }
    }

    void DrawTextWithShadow(string text, vec2i position, vec3 textColor, optional <vec3> shadowColor)
    {
        // Default shadow color
        if (shadowColor == nullopt)
            shadowColor = textColor * 0.25;

        DrawText(text, position + vec2i(ShadowOffset, -ShadowOffset) * TextScale, shadowColor.value());
        DrawText(text, position, textColor);
    }

    int GetWhitespaceWidth(char c)
    {
        if (c == ' ')
            return 1;

        if (c == '\t')
            return 4;

        return 0;
    }
}
