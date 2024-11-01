#pragma once

#include "World/Transform.h"

#include "Vertex.h"

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
        //static vector<Vertex> ToVertices(vector<Quad> quads);


        static vector <Vertex> ToVertices(vector <Quad> quads)
        {
            auto vertices = vector<Vertex>();

            for (auto quad : quads)
            {
                auto quadVertices = quad.ToVertices();
                vertices.insert(vertices.end(), quadVertices.begin(), quadVertices.end());
            }

            return vertices;
        }
    };
}
