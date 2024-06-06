#pragma once

#include "Common.h"

#include "World/Transform.h"

#include "Vertex.h"

namespace Minecraft
{
    class Vertex;

    struct Quad : public Transform
    {
        vec4 Shading = vec4(1.0f);

        vector<Vertex> ToVertices();
        //static vector<Vertex> ToVertices(vector<Quad> quads);


        static vector<Vertex> ToVertices(vector<Quad> quads)
        {
            auto vertices = vector<Vertex>();

            for (auto quad : quads)
            {
                vertices.insert_range(vertices.end(), quad.ToVertices());
            }

            return vertices;
        }
    };
}
