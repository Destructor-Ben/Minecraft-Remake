#include "Chunk.h"

#include "Game.h"
#include "LogManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/BlockData.h"

namespace Minecraft
{
    Block Chunk::GetBlock(byte blockX, byte blockY, byte blockZ)
    {
        // Can't be smaller than 0 since they're unsigned, only need to check upper bound
        Instance->Logger->Assert(blockX < Size);
        Instance->Logger->Assert(blockY < Size);
        Instance->Logger->Assert(blockZ < Size);

        return { *this, blockX, blockY, blockZ, GetBlockData(GetBlockID(blockX, blockY, blockZ)) };
    }
    
    uint Chunk::GetBlockID(byte blockX, byte blockY, byte blockZ)
    {
        return blockX * Size * Size + blockY * Size + blockZ;
    }

    BlockData& Chunk::GetBlockData(uint blockID)
    {
        return m_BlockData.at(blockID);
    }

    void Chunk::RegenerateMesh()
    {
        Instance->ChunkGraphics->RegenerateMesh(*this);
    }

    void Chunk::Tick()
    {

    }

    void Chunk::Update()
    {

    }

    void Chunk::Render()
    {
        Instance->ChunkGraphics->RenderChunk(*this);
    }
}
