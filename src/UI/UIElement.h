#pragma once

#include "Common.h"

namespace Minecraft
{
    // Basic element of UI
    class UIElement
    {
    public:
        // These are more of an output than necessarily intended to be set
        vec2i Position;
        vec2i Size;

        bool Active = true;

        UIElement() { }

        virtual void OnAdd() { }
        virtual void OnRemove() { }

        virtual void Update() { }
        virtual void Render() { }
    };
}
