#include "Vertex.h"

namespace Minecraft
{
    /*
    vector <float> Vertex::ToFloats()
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
        data.push_back(Shading.a);

        return data;
    }

    vector <float> Vertex::ToFloats(vector <Vertex> vertices)
    {
        auto data = vector<float>();

        for (auto vertex : vertices)
        {
            data.insert_range(data.end(), vertex.ToFloats());
        }

        return data;
    }

    vector <uint> Vertex::ToIndices(vector <Vertex> vertices)
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
     */
}
