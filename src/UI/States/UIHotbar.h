#pragma once

#include "UI/UIState.h"
#include "World/State/PlayerState.h"

namespace Minecraft
{
    class Texture;
    class UISprite;

    class UIHotbar : public UIState
    {
    public:
        virtual void OnInit() override;
        virtual void OnUpdate() override;
        virtual void OnRender() override;
        virtual void CheckActive() override;

    private:
        array<shared_ptr<UISprite>, PlayerState::HotbarSize> m_HotbarElements{};
        shared_ptr<Texture> m_NormalTexture;
        shared_ptr<Texture> m_SelectedTexture;
        shared_ptr<Texture> m_ChunkTexture;
    };
}
