#include "UIButton.h"

#include "Game.h"
#include "ResourceManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Input/InputManager.h"

namespace Minecraft
{
    UIButton::UIButton()
    {
        m_Texture = Instance->Resources->RequestTexture("ui/button");
    }

    void UIButton::SetButtonSize(vec2i size)
    {
        vec2i calculatedSize = size + (CornerSize * Scale + Padding) * 2;
        Width.Pixels = calculatedSize.x;
        Height.Pixels = calculatedSize.y;
    }

    void UIButton::SetText(string text)
    {
        Text.Text = text;
        SetButtonSize(TextRenderer::GetTextSize(text));
    }

    void UIButton::Update()
    {
        UIElement::Update();

        // Hover check
        m_IsHovered = GetBounds().ContainsPoint(Instance->Input->GetMousePos());

        // On mouse down and on mouse up
        // TODO: support for all 3 mouse buttons
        if (m_IsHovered && Instance->Input->WasMouseButtonPressed(MouseButton::Left) && OnMouseDown)
            OnMouseDown();

        if (m_IsHovered && Instance->Input->WasMouseButtonReleased(MouseButton::Left) && OnMouseUp)
            OnMouseUp();
    }

    void UIButton::Render()
    {
        UIElement::Render();

        // Setting up the sprite
        vec2i size = GetSize() - CornerSize * 2 * Scale; // Size of the edge sprites
        float scaledCornerSize = CornerSize * Scale;
        auto sprite = Sprite();
        sprite.SpriteTexture = m_Texture;
        //sprite.Origin = GetOrigin();
        sprite.UVs = Rectangle();
        sprite.Scale = vec2(0);

        // Calculating the UV offset for color mode + hover effects
        vec2i uvOffset = vec2i(0);

        if (m_IsHovered)
            uvOffset.x += CornerSize * 2 + EdgeSize + 1;

        // TODO: proper light + dark mode option - this code below does work, just needs a better condition
        //if (!useDarkMode)
        //    uvOffset.y += CornerSize * 2 + EdgeSize + 1;

        // TODO: calculate these sprites in CalculateBounds
        // TODO: use SetTargetRect
        // Bottom left corner
        sprite.Position = GetPosition();
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x = uvOffset.x;
        sprite.UVs->y = uvOffset.y;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = CornerSize;
        Instance->UI->DrawSprite(sprite);

        // Bottom edge
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, scaledCornerSize);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        Instance->UI->DrawSprite(sprite);

        // Bottom right corner
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        Instance->UI->DrawSprite(sprite);

        // Left edge
        sprite.Position = GetPosition() + vec2i(0, scaledCornerSize);
        sprite.Size = vec2i(scaledCornerSize, size.y);
        sprite.UVs->x = uvOffset.x;
        sprite.UVs->y = uvOffset.y + CornerSize;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = EdgeSize;
        Instance->UI->DrawSprite(sprite);

        // Middle
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, size.y);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        Instance->UI->DrawSprite(sprite);

        // Right edge
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize, size.y);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        Instance->UI->DrawSprite(sprite);

        // Top left corner
        sprite.Position = GetPosition() + vec2i(0, scaledCornerSize + size.y);
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x = uvOffset.x;
        sprite.UVs->y = uvOffset.y + CornerSize + EdgeSize;
        sprite.UVs->Width = CornerSize;
        sprite.UVs->Height = CornerSize;
        Instance->UI->DrawSprite(sprite);

        // Top edge
        sprite.Position.x += scaledCornerSize;
        sprite.Size = vec2i(size.x, scaledCornerSize);
        sprite.UVs->x += CornerSize;
        sprite.UVs->Width = EdgeSize;
        Instance->UI->DrawSprite(sprite);

        // Top right corner
        sprite.Position.x += size.x;
        sprite.Size = vec2i(scaledCornerSize);
        sprite.UVs->x += EdgeSize;
        sprite.UVs->Width = CornerSize;
        Instance->UI->DrawSprite(sprite);

        // Text
        if (Text.Text.empty())
            return;

        // TODO: do the updating in Update
        Text.Position = GetPosition();
        Text.Origin = TextRenderer::GetTextSize(Text.Text) / 2;
        TextRenderer::DrawText(Text);
    }
}
