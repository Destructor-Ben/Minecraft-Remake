#include "Quad.h"

namespace Minecraft
{
    static constexpr vec3 VertexPositions[4] = {
        { -0.5f, 0, -0.5f },
        { 0.5f,  0, -0.5f },
        { -0.5f, 0, 0.5f },
        { 0.5f,  0, 0.5f }
    };

    static constexpr vec2 VertexUVs[4] = {
        { 0, 1 },
        { 1, 1 },
        { 0, 0 },
        { 1, 0 }
    };

    void Quad::ToRawData(const vector <Quad>& quads, vector<float>& vertices, vector <uint>& indices)
    {
        uint currentIndex = 0;

        // Reserve memory - Guaranteed to know how big these will be
        uint numQuads = quads.size();
        vertices.reserve(numQuads * 4 * 11); // 4 vertices per quad, 11 floats per vertex
        indices.reserve(numQuads * 6); // 6 indices per quad

        for (const auto& quad : quads)
        {
            auto transform = quad.GetTransformationMatrix();

            // Add vertices
            for (int i = 0; i < 4; ++i)
            {
                vec4 vertexPos = transform * vec4(VertexPositions[i], 1.0f);
                vec2 vertexUVs = (VertexUVs[i] * (vec2)quad.UVs.GetSize() + (vec2)quad.UVs.GetPosition()) / (vec2)quad.TextureSize;

                vertices.insert(vertices.end(), {
                    // Position
                    vertexPos.x, vertexPos.y, vertexPos.z,
                    // Normal
                    quad.Normal.x, quad.Normal.y, quad.Normal.z,
                    // UVs
                    vertexUVs.x, vertexUVs.y,
                    // Color
                    quad.TintColor.RGB.r, quad.TintColor.RGB.g, quad.TintColor.RGB.b,
                });
            }

            // Add indices
            indices.insert(indices.end(), {
                // Triangle 1
                currentIndex + 2, currentIndex + 1, currentIndex,
                // Triangle 2
                currentIndex + 1, currentIndex + 2, currentIndex + 3,
            });

            // Increment index counter by 4 since we added 4 vertices
            currentIndex += 4;
        }
    }
}
