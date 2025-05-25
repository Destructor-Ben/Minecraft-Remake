#pragma once

#include "Graphics/TextDrawParams.h"
#include "Graphics/Texture.h"
#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UIButton : public UIElement
    {
    public:
        static constexpr int CornerSize = 5;
        static constexpr int EdgeSize = 10;
        static constexpr int Padding = 2;

        std::function<void()> OnMouseDown;
        std::function<void()> OnMouseUp;

        TextDrawParams Text;
        float Rotation = 0;
        float Depth = 0.5f;

        UIButton();

        // Accounts for padding + corner size
        void SetButtonSize(vec2i size);
        void SetButtonSize(int width, int height) { SetButtonSize(vec2i(width, height)); }
        void SetText(string text);

        bool IsHovered() const { return m_IsHovered; }

        virtual void OnUpdate() override;
        virtual void OnRender() override;
        virtual void RecalculateBounds() override;

    private:
        bool m_IsHovered = false;
        shared_ptr <Texture> m_Texture;
        array<Sprite, 9> m_Sprites = { };
    };
}
