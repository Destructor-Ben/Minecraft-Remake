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

    static void DrawTextInternal(const TextDrawParams& text);
    static int GetWhitespaceWidth(char c);

    void Init()
    {
        // Request the font
        FontTexture = Instance->Resources->RequestTexture("ui/font");

        // Init the character map
        // Letters
        for (int i = 0; i < 26; ++i)
        {
            auto rect = Rectangle(1 + 4 * i, 14, 3, 5);

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
            auto rect = Rectangle(1 + 4 * i, 8, 3, 5);
            CharacterMap['0' + i] = rect;
        }

        // Punctuation - Has to be done manually :(
        CharacterMap['.'] = Rectangle(5, 2, 1, 5);
        CharacterMap[','] = Rectangle(7, 1, 1, 6);
        CharacterMap['?'] = Rectangle(9, 2, 3, 5);
        CharacterMap['!'] = Rectangle(13, 2, 1, 5);
        CharacterMap['\''] = Rectangle(15, 2, 1, 5);
        CharacterMap['`'] = Rectangle(17, 2, 2, 5);
        CharacterMap['"'] = Rectangle(20, 2, 3, 5);
        CharacterMap['-'] = Rectangle(24, 2, 3, 5);
        CharacterMap['_'] = Rectangle(28, 2, 3, 5);
        CharacterMap['/'] = Rectangle(32, 2, 5, 5);
        CharacterMap['|'] = Rectangle(38, 2, 1, 5);
        CharacterMap['\\'] = Rectangle(40, 2, 5, 5);
        CharacterMap[':'] = Rectangle(46, 2, 1, 5);
        CharacterMap[';'] = Rectangle(48, 1, 1, 6);
        CharacterMap['('] = Rectangle(50, 2, 2, 5);
        CharacterMap[')'] = Rectangle(53, 2, 2, 5);
        CharacterMap['['] = Rectangle(56, 2, 2, 5);
        CharacterMap[']'] = Rectangle(59, 2, 2, 5);
        CharacterMap['{'] = Rectangle(62, 2, 3, 5);
        CharacterMap['}'] = Rectangle(66, 2, 3, 5);
        CharacterMap['<'] = Rectangle(70, 2, 3, 5);
        CharacterMap['>'] = Rectangle(74, 2, 3, 5);
        CharacterMap['#'] = Rectangle(78, 2, 5, 5);
        CharacterMap['^'] = Rectangle(84, 2, 3, 5);
        CharacterMap['*'] = Rectangle(88, 2, 3, 5);
        CharacterMap['+'] = Rectangle(92, 2, 3, 5);
        CharacterMap['='] = Rectangle(96, 2, 3, 5);
        CharacterMap['~'] = Rectangle(100, 2, 5, 5);
        CharacterMap['@'] = Rectangle(106, 2, 4, 5);
        CharacterMap['$'] = Rectangle(111, 1, 5, 7);
        CharacterMap['%'] = Rectangle(117, 2, 5, 5);
        CharacterMap['&'] = Rectangle(123, 2, 5, 5);
    }

    void DrawText(const TextDrawParams& text)
    {
        // Draw shadow
        if (text.HasShadow)
        {
            auto shadowText = text;
            vec2 offset = vec2(ShadowOffset, -ShadowOffset) * (vec2)TextScale * text.Scale;
            shadowText.Origin -= offset;

            // Default shadow color
            if (shadowText.ShadowColor == nullopt)
                shadowText.TextColor = text.TextColor.MultiplyRGB(0.25f);
            else
                shadowText.TextColor = shadowText.ShadowColor.value();

            DrawTextInternal(shadowText);
        }

        DrawTextInternal(text);
    }

    static void DrawTextInternal(const TextDrawParams& text)
    {
        vec2 scale = TextScale * text.Scale;
        float xOffset = 0;

        // Used to stop spacing at the beginning of the text
        bool previousCharWasWhitespace = true;

        for (char c : text.Text)
        {
            // Whitespace
            int whitespaceWidth = GetWhitespaceWidth(c);
            if (whitespaceWidth != 0)
            {
                xOffset += whitespaceWidth * WhitespaceSize * scale.x;
                previousCharWasWhitespace = true;
                continue;
            }

            // Character spacing - This is done here to avoid whitespaces being a pixel too large
            if (!previousCharWasWhitespace)
                xOffset += CharacterSpacing * scale.x;

            // Get the character UVs
            auto charUVs = UnknownCharacter;
            if (CharacterMap.contains(c))
                charUVs = CharacterMap.at(c);

            // 3 characters have a y offset: .;$
            int yOffset = 0;
            if (c == ',' || c == ';' || c == '$')
                yOffset = -scale.y;

            // Calculate rotated positions
            vec2 localPos = { xOffset, yOffset };
            vec2 offset = localPos - (vec2)text.Origin;
            vec2 rotatedOffset = glm::rotate(offset, text.Rotation);
            vec2i worldPos = text.Position + (vec2i)rotatedOffset;

            // Draw the sprite
            auto targetRect = Rectangle(worldPos, (vec2)charUVs.GetSize() * scale);
            auto sprite = Sprite();
            sprite.SetTargetRect(targetRect);
            sprite.Origin = vec2i(0);
            sprite.Rotation = text.Rotation;
            sprite.Depth = text.Depth;
            sprite.SpriteTexture = FontTexture;
            sprite.SpriteColor = text.TextColor;
            sprite.UVs = charUVs;
            Instance->UI->DrawSprite(sprite);

            // Advance the position
            xOffset += charUVs.Width * scale.x; // Character size
            previousCharWasWhitespace = false;
        }
    }

    vec2i GetTextSize(const string& text, vec2 scale)
    {
        // The default height is based on the font
        // Some characters are 7 tall but that fucks with centering
        // since not all characters use the extra pixel above
        // and the one below is under the baseline
        vec2i size = vec2i(0, 5 * TextScale);

        // Used to stop spacing at the beginning of the text
        bool previousCharWasWhitespace = true;

        for (char c : text)
        {
            // Whitespace
            int whitespaceWidth = GetWhitespaceWidth(c);
            if (whitespaceWidth != 0)
            {
                size.x += whitespaceWidth * WhitespaceSize * TextScale;
                previousCharWasWhitespace = true;
                continue;
            }

            // Character spacing - This is done here to avoid whitespaces being a pixel too large
            if (!previousCharWasWhitespace)
                size.x += CharacterSpacing * TextScale;

            // Get the character UVs
            auto charUVs = UnknownCharacter;
            if (CharacterMap.contains(c))
                charUVs = CharacterMap.at(c);

            // Increase the size
            size.x += charUVs.Width * TextScale; // Character size
            previousCharWasWhitespace = false;
        }

        return (vec2)size * scale;
    }

    static int GetWhitespaceWidth(char c)
    {
        if (c == ' ')
            return 1;

        if (c == '\t')
            return 4;

        return 0;
    }
}
