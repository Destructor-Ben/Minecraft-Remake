#pragma once

#include "Common.h"

#include "World/Transform.h"

namespace Minecraft
{
    class Vertex;

    struct Quad : public Transform
    {
        vec4 Shading = vec4(1.0f);

        vector<Vertex> ToVertices();
        static vector<Vertex> ToVertices(vector<Quad> quads);
    };
}
