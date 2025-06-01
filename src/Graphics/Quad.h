#pragma once

#include "Colors.h"
#include "Rectangle.h"
#include "Transform.h"

namespace Minecraft
{
    struct Quad : public Transform
    {
        vec3 Normal = { };
        vec2i TextureSize = { 1, 1 }; // Used with the UVs rect to calculate the actual UV coords
        Rectangle UVs = Rectangle(1, 1);
        Color TintColor = Colors::White;

        static void ToRawData(const vector <Quad>& quads, vector<float>& vertices, vector <uint>& indices);
    };
}
