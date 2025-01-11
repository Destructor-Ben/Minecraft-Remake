#pragma once

namespace Minecraft
{
    struct Vertex
    {
        vec3 Position = vec3(0.0f);
        vec2 UV = vec2(0.0f);
        // TODO: more elegant way of handling extra data
        vec3 Shading = vec4(1.0f);

        // TODO: fix dodgy linker error
        //vector<float> ToFloats();
        //static vector<float> ToFloats(vector<Vertex> vertices);

        // Assumes that each set of 4 vertices is a quad
        //static vector<uint> ToIndices(vector<Vertex> vertices);

        vector<float> ToFloats()
        {
            auto data = vector<float>();

            data.push_back(Position.x);
            data.push_back(Position.y);
            data.push_back(Position.z);

            data.push_back(UV.x);
            data.push_back(UV.y);

            data.push_back(Shading.r);
            data.push_back(Shading.g);
            data.push_back(Shading.b);

            return data;
        }

        static vector<float> ToFloats(vector<Vertex> vertices)
        {
            auto data = vector<float>();

            for (auto vertex : vertices)
            {
                auto vertexFloats = vertex.ToFloats();
                data.insert(data.end(), vertexFloats.begin(), vertexFloats.end());
            }

            return data;
        }

        static vector<uint> ToIndices(vector<Vertex> vertices)
        {
            auto indices = vector<uint>();

            for (int i = 0; i < vertices.size(); i += 4)
            {
                uint index0 = i;
                uint index1 = i + 1;
                uint index2 = i + 2;
                uint index3 = i + 3;

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
