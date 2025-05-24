#pragma once

#include "Rectangle.h"
#include "UI/UIDimension.h"

namespace Minecraft
{
    // Basic element of UI
    class UIElement
    {
    public:
        UIElement* Parent = nullptr;

        UIDimension x;
        UIDimension y;

        UIDimension Width;
        UIDimension Height;

        UIDimension OriginX;
        UIDimension OriginY;

        bool Active = true;

        UIElement() { }

        Rectangle GetParentBounds() const;
        Rectangle GetBounds() const { return m_Bounds; }
        vec2i GetPosition() const { return m_Bounds.GetPosition(); }
        vec2i GetSize() const { return m_Bounds.GetSize(); }
        vec2i GetOrigin() const { return m_Origin; }

        virtual void CalculateBounds();

        virtual void OnAdd() { }
        virtual void OnRemove() { }

        virtual void Update() { }
        virtual void Render() { }

    private:
        Rectangle m_Bounds;
        vec2i m_Origin;
    };
}
