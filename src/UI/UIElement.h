#pragma once

#include "Rectangle.h"

namespace Minecraft
{
    // Basic element of UI
    class UIElement
    {
    public:
        // These are more of an output than necessarily intended to be set
        // TODO: make these a custom unit based on screen width
        vec2i Position = vec2i(0);
        vec2i Origin = vec2i(0);
        vec2i Size = vec2i(1);

        bool Active = true;

        UIElement() { }

        virtual Rectangle GetBounds();

        virtual void OnAdd() { }
        virtual void OnRemove() { }

        virtual void Update() { }
        virtual void Render() { }
    };
}
