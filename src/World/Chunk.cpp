#include "Chunk.h"

#include "BoundingBox.h"
#include "Logger.h"
#include "World/BlockData.h"

namespace Minecraft
{
    uint Chunk::GetBlockID(const BlockOffset& blockOffset)
    {
        // XYZ order here, so z changes fastest, meaning XYZ order in for loops
        // TODO: consider changing this
        return (blockOffset.x << 8) + (blockOffset.y << 4) + blockOffset.z;
    }

    Block Chunk::GetBlock(const BlockOffset& blockOffset)
    {
        // Can't be smaller than 0 since they're unsigned, only need to check upper bound
        // Bitwise operations are very fast :)
        Logger::Assert((blockOffset.x & 15) == blockOffset.x &&
                       (blockOffset.y & 15) == blockOffset.y &&
                       (blockOffset.z & 15) == blockOffset.z);

        return { this, blockOffset, GetBlockData(GetBlockID(blockOffset)) };
    }

    BlockData* Chunk::GetBlockData(uint blockID)
    {
        return &m_BlockData.at(blockID);
    }

    bool Chunk::ContainsPos(const vec3& worldPos) const
    {
        auto pos = worldPos - GetWorldPos();
        return pos.x >= 0 && pos.x < Chunk::Size &&
               pos.y >= 0 && pos.y < Chunk::Size &&
               pos.z >= 0 && pos.z < Chunk::Size;
    }

    bool Chunk::ContainsPos(const BlockPos& blockPos) const
    {
        auto pos = blockPos.ToWorldPos() - GetWorldPos();
        return pos.x >= 0 && pos.x < Chunk::Size &&
               pos.y >= 0 && pos.y < Chunk::Size &&
               pos.z >= 0 && pos.z < Chunk::Size;
    }

    bool Chunk::ContainsPos(const BlockOffset& pos) const
    {
        return pos.x >= 0 && pos.x < Chunk::Size &&
               pos.y >= 0 && pos.y < Chunk::Size &&
               pos.z >= 0 && pos.z < Chunk::Size;
    }

    BoundingBox Chunk::GetBounds() const
    {
        auto corner = m_ChunkPos.ToWorldPos();
        return BoundingBox(corner, corner + vec3(Size));
    }
}
