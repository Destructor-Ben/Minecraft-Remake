#pragma once

#include "src/Common.h"

namespace Minecraft
{
    struct Vertex
    {
        vec3 Position = vec3(0.0f);
        vec2 UV = vec2(0.0f);
        vec4 Shading = vec4(1.0f);
    };
}
