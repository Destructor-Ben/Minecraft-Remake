#pragma once

namespace Minecraft
{
    struct Vertex
    {
        vec3 Position = vec3(0.0f);
        vec2 UV = vec2(0.0f);
        // TODO: more elegant way of handling extra data
        vec4 Shading = vec4(1.0f);

        vector<float32> ToFloats();
        static vector<float32> ToFloats(vector<Vertex> vertices);

        // Assumes that each set of 4 vertices is a quad
        static vector<uint32> ToIndices(vector<Vertex> vertices);
    };
}
