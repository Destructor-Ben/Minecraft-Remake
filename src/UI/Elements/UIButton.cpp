#include "UIButton.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Input/InputManager.h"
#include "UI/UI.h"

namespace Minecraft
{
    UIButton::UIButton()
    {
        m_Texture = Instance->Resources->RequestTexture("ui/button");
    }

    void UIButton::SetButtonSize(vec2i size)
    {
        vec2i calculatedSize = size + (CornerSize + Padding) * 2 * UI::SpriteScale;
        Width.Pixels = calculatedSize.x;
        Height.Pixels = calculatedSize.y;
        RecalculateBounds();
    }

    void UIButton::SetText(string text)
    {
        Text.Text = text;
        SetButtonSize(TextRenderer::GetTextSize(text));
    }

    void UIButton::SetDepth(float depth)
    {
        m_Depth = depth;
        RecalculateBounds();
    }

    // At some point, this input code will get moved to UIElement
    void UIButton::OnUpdate()
    {
        // Hover check
        m_IsHovered = GetBounds().ContainsPoint(Instance->Input->GetMousePos());

        // On mouse down and on mouse up
        if (m_IsHovered && Instance->Input->WasMouseButtonPressed(MouseButton::Left) && OnMouseDown)
            OnMouseDown();

        if (m_IsHovered && Instance->Input->WasMouseButtonReleased(MouseButton::Left) && OnMouseUp)
            OnMouseUp();
    }

    void UIButton::OnRender()
    {
        // Background
        for (auto sprite : m_Sprites)
        {
            // Calculating the UV offset for color mode + hover effects
            if (m_IsHovered)
                sprite.UVs->x += CornerSize * 2 + EdgeSize + 1;

            if (UI::IsInLightMode)
                sprite.UVs->y += CornerSize * 2 + EdgeSize + 1;

            // Draw
            Instance->UI->DrawSprite(sprite);
        }

        // Text
        if (Text.Text.empty())
            return;

        TextRenderer::DrawText(Text);
    }

    // Recalculate the sprites
    void UIButton::RecalculateBounds()
    {
        UIElement::RecalculateBounds();

        // Setting up the sprite
        int scaledCornerSize = CornerSize * UI::SpriteScale;
        vec2i size = GetSize() - scaledCornerSize * 2; // Size of the contents and padding

        auto sprite = Sprite();
        sprite.SpriteTexture = m_Texture;
        sprite.Origin = GetOrigin();
        sprite.UVs = Rectangle();
        sprite.Depth = m_Depth;

        // Bottom left corner
        sprite.Position = GetPosition();
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x = 0;
        sprite.UVs->y = 0;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = CornerSize;
        m_Sprites[0] = sprite;

        // Bottom edge
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, scaledCornerSize);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        m_Sprites[1] = sprite;

        // Bottom right corner
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        m_Sprites[2] = sprite;

        // Left edge
        sprite.Position = GetPosition() + vec2i(0, scaledCornerSize);
        sprite.Size = vec2i(scaledCornerSize, size.y);
        sprite.UVs->x = 0;
        sprite.UVs->y = CornerSize;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = EdgeSize;
        m_Sprites[3] = sprite;

        // Middle
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, size.y);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        m_Sprites[4] = sprite;

        // Right edge
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize, size.y);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        m_Sprites[5] = sprite;

        // Top left corner
        sprite.Position = GetPosition() + vec2i(0, scaledCornerSize + size.y);
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x = 0;
        sprite.UVs->y = CornerSize + EdgeSize;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = CornerSize;
        m_Sprites[6] = sprite;

        // Top edge
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, scaledCornerSize);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        m_Sprites[7] = sprite;

        // Top right corner
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        m_Sprites[8] = sprite;

        // Text - Always centered
        Text.Position = GetPosition() - GetOrigin() + GetSize() / 2;
        Text.Origin = TextRenderer::GetTextSize(Text.Text) / 2;
        Text.Depth = m_Depth;
    }
}
