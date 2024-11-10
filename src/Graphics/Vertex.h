#pragma once

namespace Minecraft
{
    struct Vertex
    {
        vec3 Position = vec3(0.0f);
        vec2 UV = vec2(0.0f);
        // TODO: more elegant way of handling extra data
        vec4 Shading = vec4(1.0f);

        // TODO: fix dodgy linker error
        //vector<float32> ToFloats();
        //static vector<float32> ToFloats(vector<Vertex> vertices);

        // Assumes that each set of 4 vertices is a quad
        //static vector<uint32> ToIndices(vector<Vertex> vertices);

        vector <float32> ToFloats()
        {
            auto data = vector<float32>();

            data.push_back(Position.x);
            data.push_back(Position.y);
            data.push_back(Position.z);

            data.push_back(UV.x);
            data.push_back(UV.y);

            data.push_back(Shading.r);
            data.push_back(Shading.g);
            data.push_back(Shading.b);
            data.push_back(Shading.a);

            return data;
        }

        static vector <float32> ToFloats(vector <Vertex> vertices)
        {
            auto data = vector<float32>();

            for (auto vertex : vertices)
            {
                auto vertexFloats = vertex.ToFloats();
                data.insert(data.end(), vertexFloats.begin(), vertexFloats.end());
            }

            return data;
        }

        static vector <uint32> ToIndices(vector <Vertex> vertices)
        {
            auto indices = vector<uint32>();

            for (int32 i = 0; i < vertices.size(); i += 4)
            {
                uint32 index0 = i;
                uint32 index1 = i + 1;
                uint32 index2 = i + 2;
                uint32 index3 = i + 3;

                indices.push_back(index2);
                indices.push_back(index1);
                indices.push_back(index0);

                indices.push_back(index1);
                indices.push_back(index2);
                indices.push_back(index3);
            }

            return indices;
        }
    };
}
