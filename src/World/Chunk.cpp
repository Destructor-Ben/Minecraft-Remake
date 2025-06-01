#include "Chunk.h"

#include "Game.h"
#include "Logger.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "World/BlockData.h"

namespace Minecraft
{
    bool Chunk::ContainsBlockPos(int blockX, int blockY, int blockZ)
    {
        if (blockX < 0 || blockX >= Chunk::Size)
            return false;

        if (blockY < 0 || blockY >= Chunk::Size)
            return false;

        if (blockZ < 0 || blockZ >= Chunk::Size)
            return false;

        return true;
    }

    Block Chunk::GetBlock(byte blockX, byte blockY, byte blockZ)
    {
        // Can't be smaller than 0 since they're unsigned, only need to check upper bound
        Logger::Assert(blockX < Size);
        Logger::Assert(blockY < Size);
        Logger::Assert(blockZ < Size);

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

    void Chunk::Tick()
    {

    }

    void Chunk::Update()
    {

    }
}
