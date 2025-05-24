#include "UICrosshair.h"

#include "Game.h"
#include "ResourceManager.h"
#include "UI/Elements/UISprite.h"

namespace Minecraft
{
    void UICrosshair::Init()
    {
        auto texture = Instance->Resources->RequestTexture("ui/crosshair");
        m_Crosshair = make_shared<UISprite>();
        m_Crosshair->DrawnSprite.SpriteTexture = texture;
        m_Crosshair->DrawnSprite.Scale = vec2(3.0f); // TODO: keep a global sprite scale property somehwere, proibably UIRenderer
        m_Crosshair->x.Percent = 0.5f;
        m_Crosshair->y.Percent = 0.5f;
        m_Crosshair->OriginX.Percent = 0.5f;
        m_Crosshair->OriginX.Percent = 0.5f;

        AddElement(m_Crosshair);

        UIState::Init();
    }

    void UICrosshair::CheckActive()
    {
        Active = !Instance->IsPaused;

        UIState::CheckActive();
    }
}
