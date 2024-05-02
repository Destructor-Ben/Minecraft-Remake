#pragma once

#include "src/Common.h"
#include "Vertex.h"
#include "src/World/Transform.h"

namespace Minecraft
{
    struct Quad : public Transform
    {
        std::vector<Vertex> ToVertices();
    };
}
