#include "Quad.h"

#include "Graphics/Vertex.h"

namespace Minecraft
{
    vector<Vertex> Quad::ToVertices()
    {
        vector<Vertex> vertices { };

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
            vertices.push_back(vertex);
        }

        return vertices;
    }
/*
    vector<Vertex> Quad::ToVertices(vector<Quad> quads)
    {
        auto vertices = vector<Vertex>();

        for (auto quad : quads)
        {
            vertices.insert_range(vertices.end(), quad.ToVertices());
        }

        return vertices;
    }
    */
}
