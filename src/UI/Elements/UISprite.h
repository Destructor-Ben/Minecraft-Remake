#pragma once

#include "Graphics/Sprite.h"
#include "UI/UIElement.h"

namespace Minecraft
{
    class UISprite : public UIElement
    {
    public:
        // TODO: just use a sprite variable
        float Rotation = 0;
        float Depth = 0.5f;
        vec2 Scale = vec2(1); // Scale * TextureSize gets outputted into Size

        optional <Rectangle> UVs = nullopt;

        vec3 Color = vec3(1);
        float Opacity = 1.0f;
        shared_ptr <Texture> SpriteTexture;

        virtual void Update() override;
        virtual void Render() override;
    };
}
