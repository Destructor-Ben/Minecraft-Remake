#pragma once

#include "Transform.h"

// TODO: TEMPORARY FOR HACKING TOGETHER
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

        // TODO: fix dodgy linker error
        //vector <Vertex> ToVertices();
        //static vector <Vertex> ToVertices(vector <Quad> quads);


        vector <Vertex> ToVertices()
        {
            vector <Vertex> vertices { };

            for (int32 i = 0; i < 4; ++i)
            {
                Vertex vertex { };

                switch (i)
                {
                    case 0:
                        vertex.Position = vec3(-0.5f, 0, -0.5f);
                        vertex.UV = vec2(0, 1);
                        break;
                    case 1:
                        vertex.Position = vec3(0.5f, 0, -0.5f);
                        vertex.UV = vec2(1, 1);
                        break;
                    case 2:
                        vertex.Position = vec3(-0.5f, 0, 0.5f);
                        vertex.UV = vec2(0, 0);
                        break;
                    case 3:
                        vertex.Position = vec3(0.5f, 0, 0.5f);
                        vertex.UV = vec2(1, 0);
                        break;
                    default:
                        break;
                }

                vertex.Position = vec3(GetTransformationMatrix() * vec4(vertex.Position, 1.0f));
                vertex.Shading = Shading;
                vertex.UV *= UVMultiplier;
                vertex.UV += UVOffset;
                vertices.push_back(vertex);
            }

            return vertices;
        }

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
