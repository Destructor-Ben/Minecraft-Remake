#include "UIHotbar.h"

#include "Game.h"
#include "Graphics/Renderers/UIRenderer.h"
#include "Graphics/Sprite.h"
#include "Rectangle.h"
#include "World/World.h"
#include "ResourceManager.h"
#include "UI/UI.h"
#include "UI/Elements/UISprite.h"
#include "World/State/PlayerState.h"

namespace Minecraft
{
    static shared_ptr<UISprite> CreateHotbarSlot(int index, int textureWidth)
    {
        auto element = make_shared<UISprite>();
        element->x.Percent = 0.5f;
        element->x.Pixels = (index - 5) * textureWidth * UI::SpriteScale;
        element->y.Pixels = 25;
        return element;
    }

    void UIHotbar::OnInit()
    {
        m_NormalTexture = Resources::RequestTexture("ui/hotbar");
        m_SelectedTexture = Resources::RequestTexture("ui/hotbar-active");
        m_ChunkTexture = Resources::RequestTexture("chunk");

        for (int i = 0; i < PlayerState::HotbarSize; i++)
        {
            // TODO: add hotbar numbers to make it easy to know which number to press
            // TODO: fix the double pixel gaps between the hotbar slots
            // TODO: ensure centred
            m_HotbarElements[i] = CreateHotbarSlot(i, m_NormalTexture->GetWidth());
            AddElement(m_HotbarElements[i]);
        }
    }

    void UIHotbar::OnUpdate()
    {
        // TODO: pass in
        auto &player = Instance->CurrentWorld->Player;

        for (int i = 0; i < PlayerState::HotbarSize; i++)
        {
            auto element = m_HotbarElements[i];
            auto texture = i == player.SelectedHotbarSlot ? m_SelectedTexture : m_NormalTexture;
            element->SetTexture(texture, vec2(UI::SpriteScale));
        }
    }

    void UIHotbar::OnRender()
    {
        // TODO: pass in
        auto &player = Instance->CurrentWorld->Player;

        for (int i = 0; i < PlayerState::HotbarSize; i++)
        {
            auto element = m_HotbarElements[i];
            auto item = player.Hotbar[i];
            
            if (!item.has_value())
                continue;

            // TODO: temporary until I have proper item rendering code
            auto blockType = item.value();
            vec2i textureCoord = blockType->GetTextureCoords(vec3i(0, 0, 1));
            int chunkTextureSize = m_ChunkTexture->GetWidth();
            int blockTextureSize = 16;
            int blockCount = chunkTextureSize / blockTextureSize;

            Rectangle uvs(
                textureCoord.x * blockTextureSize,
                chunkTextureSize - (textureCoord.y + 1) * blockTextureSize,
                blockTextureSize,
                blockTextureSize
            );

            vec2i spriteSizeDifference = m_NormalTexture->GetSize() - blockTextureSize;

            Sprite sprite {
                .Position = element->GetPosition() + element->GetSize() / 2 + spriteSizeDifference * UI::SpriteScale / 2,
                .Origin = element->GetSize() / 2,
                // TODO: fix this drawing in front of the pause menu
                .Depth = 0.4f, // In front of slots
                .UVs = uvs,
            };

            sprite.SetTextureAndScale(m_ChunkTexture, vec2(UI::SpriteScale / (float)blockCount));

            UIRenderer::DrawSprite(sprite);
        }
    }

    void UIHotbar::CheckActive()
    {
        Active = true;

        UIState::CheckActive();
    }
}
