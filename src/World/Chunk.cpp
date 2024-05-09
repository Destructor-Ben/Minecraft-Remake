#include "Chunk.h"

#include "Game.h"

namespace Minecraft
{
    Block Chunk::GetBlock(uint8 blockX, uint8 blockY, uint8 blockZ)
    {
        // Can't be smaller than 0 since they're unsigned, only need to check upper bound
        Logger->Assert(blockX < Size);
        Logger->Assert(blockY < Size);
        Logger->Assert(blockZ < Size);

        return {*this, blockX, blockY, blockZ};
    }

    void Chunk::RegenerateMesh()
    {
        Renderer->ChunkRenderer->RegenerateMesh(*this);
    }

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
