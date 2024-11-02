#pragma once

#include "Transform.h"

namespace Minecraft
{
    class Vertex;

    struct Quad : public Transform
    {
        // TODO: more elegant way of handling extra data
        vec4 Shading = vec4(1.0f);
        vec2 UVMultiplier = vec2(1.0f);
        vec2 UVOffset = vec2(0.0f);

        vector <Vertex> ToVertices();
        static vector <Vertex> ToVertices(vector <Quad> quads);
    };
}
