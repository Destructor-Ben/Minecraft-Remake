#include "Quad.h"

namespace Minecraft
{
    std::vector<Vertex> Quad::ToVertices()
    {
        std::vector<Vertex> vertices;

        for (int i = 0; i < 4; ++i)
        {
            Vertex vertex;

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
}
