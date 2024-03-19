#pragma once

#include "../Common.h"

#include "Object.h"

namespace Minecraft
{
    // Handles not only transformations but also update methods
    class Entity : public Object
    {
    public:
        Entity() : Object() { }

        virtual void Tick() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
    };
}
