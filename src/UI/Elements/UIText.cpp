#include "UIText.h"

#include "Graphics/Renderers/TextRenderer.h"

namespace Minecraft
{
    void UIText::SetText(string text)
    {
        vec2i size = TextRenderer::GetTextSize(text);
        Width.Pixels = size.x;
        Height.Pixels = size.y;
        Text.Text = text;
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
