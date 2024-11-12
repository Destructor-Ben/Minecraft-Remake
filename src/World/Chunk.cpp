#include "Chunk.h"

#include "Game.h"
#include "LogManager.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"

namespace Minecraft
{
    Block Chunk::GetBlock(byte blockX, byte blockY, byte blockZ)
    {
        // Can't be smaller than 0 since they're unsigned, only need to check upper bound
        Logger->Assert(blockX < Size);
        Logger->Assert(blockY < Size);
        Logger->Assert(blockZ < Size);

        return { *this, blockX, blockY, blockZ };
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
