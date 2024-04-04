#include "Chunk.h"

#include "../Game.h"

namespace Minecraft
{
    Chunk::Chunk(int32 x, int32 y, int32 z): X(x), Y(y), Z(z)
    {

    }

    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ) const
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
        for (int x = 0; x < Size; x++)
        {
            for (int y = 0; y < Size; y++)
            {
                for (int z = 0; z < Size; z++)
                {
                    Block block = GetBlock(x, y, z);
                    if (block.GetData().Type == BlockType::Air)
                        continue;

                    Transform transform;
                    transform.Position = vec3(block.GetX(), block.GetY(), block.GetZ());
                    Renderer->Draw(*World->mesh, transform.GetTransformationMatrix());
                }
            }
        }
    }
}
