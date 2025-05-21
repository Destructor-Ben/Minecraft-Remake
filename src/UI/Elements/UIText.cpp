#include "UIText.h"

#include "Graphics/Renderers/TextRenderer.h"

namespace Minecraft
{
    void UIText::SetText(string text)
    {
        Text.Text = text;
        Size = TextRenderer::GetTextSize(text);
    }

    void UIText::Render()
    {
        UIElement::Render();

        // TODO: do the updating in Update
        Text.Position = Position;
        Text.Origin = Origin;
        TextRenderer::DrawText(Text);
    }
}
