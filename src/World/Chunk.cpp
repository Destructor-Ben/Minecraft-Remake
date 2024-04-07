#include "Chunk.h"

#include "../Game.h"

namespace Minecraft
{
    Chunk::Chunk(int32 x, int32 y, int32 z): X(x), Y(y), Z(z)
    {
        RegenerateMesh();
    }

    Chunk::~Chunk()
    {
        DeleteMesh();
    }

    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ)
    {
        return Block(*this, localX, localY, localZ);
    }

    /*/
    template<typename T>
    T& Chunk::GetBlockData(Block block)
    {
        return m_BlockData[T::GetID()][block.GetID()];
    }

    template<typename T>
    T& Chunk::AddBlockData(Block block)
    {
        //m_BlockData[block.GetID()] = T();
        return GetBlockData<T>(block);
    }

    template<typename T>
    void Chunk::RemoveBlockData(Block block)
    {
        //m_BlockData[T::GetID()].erase(block.GetID());
    }
    //*/

    void Chunk::Tick()
    {

    }

    void Chunk::Update()
    {

    }

    void Chunk::Render()
    {
        Transform transform;
        transform.Position = vec3(X * Size, Y * Size, Z * Size);
        Renderer->Draw(*m_Mesh, transform.GetTransformationMatrix());
    }

    // TODO: chunk meshing class
    // TODO: implement greedy meshing - flat, adjacent block faces use the same quad
    void Chunk::RegenerateMesh()
    {
        // Deleting old mesh
        DeleteMesh();

        // Adding faces
        struct Face
        {
            glm::vec3 Centre;
            glm::vec3 Normal;
        };

        std::vector<Face> faces;

        // TODO: make this work with neighboring chunks - don't forget to update nearby chunks
        for (int x = 0; x < Size; ++x)
        {
            for (int y = 0; y < Size; ++y)
            {
                for (int z = 0; z < Size; ++z)
                {
                    Block block = GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    Face face{};
                    face.Centre = block.GetPosition();
                    face.Normal = vec3(0.0f, 1.0f, 0.0f);
                    faces.push_back(face);
                }
            }
        }

        // Generating the vertices
        auto vertices = new std::vector<float>();
        auto indices = new std::vector<uint32>();
        for (Face face : faces)
        {
            // Vertices
            // TODO: properly handle the rotation
            vec3 centre = face.Centre;
            vertices->push_back(centre.x - 0.5f);
            vertices->push_back(centre.y);
            vertices->push_back(centre.z - 0.5f);

            vertices->push_back(centre.x + 0.5f);
            vertices->push_back(centre.y);
            vertices->push_back(centre.z - 0.5f);

            vertices->push_back(centre.x - 0.5f);
            vertices->push_back(centre.y);
            vertices->push_back(centre.z + 0.5f);

            vertices->push_back(centre.x + 0.5f);
            vertices->push_back(centre.y);
            vertices->push_back(centre.z + 0.5f);

            // Indices
            uint32 vertexCount = vertices->size() / sizeof(float);
            uint32 index0 = vertexCount - 3;
            uint32 index1 = vertexCount - 2;
            uint32 index2 = vertexCount - 1;
            uint32 index3 = vertexCount;

            indices->push_back(index2);
            indices->push_back(index1);
            indices->push_back(index0);

            indices->push_back(index1);
            indices->push_back(index2);
            indices->push_back(index3);
        }

        // Creating the mesh
        m_VertexBuffer = new VertexBuffer();
        m_VertexBuffer->SetData(&(*vertices)[0], vertices->size());

        m_IndexBuffer = new IndexBuffer();
        m_IndexBuffer->SetData(&(*indices)[0], indices->size() / sizeof(uint32));

        m_VertexArray = new VertexArray();
        m_VertexArray->Push(GL_FLOAT, 3);
        m_VertexArray->Push(GL_FLOAT, 2);
        m_VertexArray->AddBuffer(*m_VertexBuffer);

        m_Mesh = new Mesh(*m_VertexArray);
        m_Mesh->AddMaterial(World->material, m_IndexBuffer);

        // Delete temporary objects
        delete vertices;
        delete indices;
    }

    void Chunk::DeleteMesh()
    {
        if (!m_Mesh)
            delete m_Mesh;

        if (!m_VertexArray)
            delete m_VertexArray;

        if (!m_VertexBuffer)
            delete m_VertexBuffer;

        if (!m_IndexBuffer)
            delete m_IndexBuffer;
    }
}
