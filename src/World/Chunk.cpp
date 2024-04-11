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
        auto faces = GetFaces();

        // Generating the vertices
        if (faces.empty())
            return;

        // TODO: move this into a function
        auto vertices = std::vector<float>();
        auto indices = std::vector<uint32>();
        for (Face face : faces)
        {
            // Vertices
            // TODO: properly handle the rotation with the normal
            vec3 centre = face.Centre;
            vertices.push_back(centre.x - 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z - 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);

            vertices.push_back(centre.x + 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z - 0.5f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);

            vertices.push_back(centre.x - 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z + 0.5f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(centre.x + 0.5f);
            vertices.push_back(centre.y);
            vertices.push_back(centre.z + 0.5f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Indices
            // TODO: use a vertex struct to make this maths less convoluted
            uint32 vertexCount = vertices.size() / 5;
            uint32 index0 = vertexCount - 4;
            uint32 index1 = vertexCount - 3;
            uint32 index2 = vertexCount - 2;
            uint32 index3 = vertexCount - 1;

            indices.push_back(index2);
            indices.push_back(index1);
            indices.push_back(index0);

            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
        }
        // End TODO

        CreateMesh(vertices, indices);
    }

    void Chunk::CreateMesh(const std::vector<float>& vertices, const std::vector<uint32>& indices)
    {
        DeleteMesh();

        m_VertexBuffer = new VertexBuffer();
        m_VertexBuffer->SetData(vertices.data(), vertices.size());

        m_IndexBuffer = new IndexBuffer();
        m_IndexBuffer->SetData(indices.data(), indices.size());

        m_VertexArray = new VertexArray();
        m_VertexArray->Push(GL_FLOAT, 3);
        m_VertexArray->Push(GL_FLOAT, 2);
        m_VertexArray->AddBuffer(*m_VertexBuffer);

        m_Mesh = new Mesh(*m_VertexArray);

        // TODO: make the material less convoluted
        m_Shader = new Shader(Shader::FromFile("assets/shaders/shader"));
        m_Material = new Material(*m_Shader);

        int width, height, channels;
        uint8* data = stbi_load("assets/textures/test.png", &width, &height, &channels, 0);
        if (!data)
        {
            throw std::exception();
        }

        m_Texture = new Texture();
        m_Texture->SetData(data, width, height, GL_RGBA);
        stbi_image_free(data);

        m_Mesh->AddMaterial(m_Material, m_IndexBuffer);
        // End TODO
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

        // TODO Temporary
        if (!m_Texture)
            delete m_Texture;

        if (!m_Shader)
            delete m_Shader;

        if (!m_Material)
            delete m_Material;
    }

    std::vector<Chunk::Face> Chunk::GetFaces()
    {
        std::vector<Face> faces;

        // TODO: make this work with neighboring chunks - don't forget to update nearby chunks
        // TODO: make this add faces properly
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
                    face.Centre = block.GetChunkPos();
                    face.Normal = vec3(0.0f, 1.0f, 0.0f);
                    faces.push_back(face);
                }
            }
        }

        return faces;
    }
}
