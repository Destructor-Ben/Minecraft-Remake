#pragma once

#include "../Common.h"

#include "Transform.h"

namespace Minecraft
{
    // Handles not only transformations but also update methods
    class Entity : public Transform
    {
    public:
        Entity() : Transform() { }

        virtual void Tick() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
    };
}
