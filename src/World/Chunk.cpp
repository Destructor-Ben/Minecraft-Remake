#include "Chunk.h"

#include "Game.h"
#include "Graphics/Renderer.h"

namespace Minecraft
{
    Chunk::Chunk(int32 x, int32 y, int32 z) : X(x), Y(y), Z(z)
    {
    }

    Block Chunk::GetBlock(uint8 localX, uint8 localY, uint8 localZ)
    {
        // TODO: should this be moved to another place?
        localX = std::clamp<uint8>(localX, 0, Chunk::Size - 1);
        localY = std::clamp<uint8>(localY, 0, Chunk::Size - 1);
        localZ = std::clamp<uint8>(localZ, 0, Chunk::Size - 1);
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
        Renderer->ChunkRenderer->RenderChunk(*this);
    }
}
