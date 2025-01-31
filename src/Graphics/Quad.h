#pragma once

#include "Transform.h"

namespace Minecraft
{
    struct Quad : public Transform
    {
        vec2 UVPosition = vec2(0.0f);
        vec2 UVScale = vec2(1.0f);
        vec3 Shading = vec4(1.0f);

        static void ToRawData(const vector <Quad>& quads, vector<float>& vertices, vector <uint>& indices);
    };
}
