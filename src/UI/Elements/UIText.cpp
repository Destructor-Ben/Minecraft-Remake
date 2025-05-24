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
        CalculateBounds();
    }

    void UIText::CalculateBounds()
    {
        UIElement::CalculateBounds();

        Text.Position = GetPosition();
        Text.Origin = GetOrigin();
    }

    void UIText::Render()
    {
        UIElement::Render();

        TextRenderer::DrawText(Text);
    }
}
