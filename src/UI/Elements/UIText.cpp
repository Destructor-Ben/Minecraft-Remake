#include "UIText.h"

#include "Graphics/Renderers/TextRenderer.h"

namespace Minecraft
{
    void UIText::SetText(string text, vec2 scale)
    {
        Text.Text = text;
        Text.Scale = scale;

        // Set size of the UI element
        vec2i size = TextRenderer::GetTextSize(text, scale);
        Width.Pixels = size.x;
        Height.Pixels = size.y;
        RecalculateBounds();
    }

    void UIText::RecalculateBounds()
    {
        UIElement::RecalculateBounds();

        Text.Position = GetPosition();
        Text.Origin = GetOrigin();
    }

    void UIText::OnRender()
    {
        TextRenderer::DrawText(Text);
    }
}
