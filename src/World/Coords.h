#pragma once

namespace Minecraft
{
    // World pos is a generic floating point position in the world
    // All the conversions assume chunk sizes are 16 (since they are optimized for sizes that are powers of 2)
    // Unions are used to make accessing x, y, z, and pos easy
    // TODO: test all of the conversion functions

    // Position of a block in the world
    struct BlockPos
    {
        union
        {
            vec3i Pos;
            struct
            {
                int x;
                int y;
                int z;
            };
        };

        BlockPos() : x(0), y(0), z(0) { }

        explicit BlockPos(vec3i pos)
        {
            Pos = pos;
        }

        BlockPos(int x, int y, int z)
        {
            Pos.x = x;
            Pos.y = y;
            Pos.z = z;
        }

        vec3 ToWorldPos() const
        {
            return vec3(Pos);
        }

        static BlockPos FromWorldPos(vec3 worldPos)
        {
            return BlockPos(glm::floor(worldPos));
        }
    };

    // Position of a chunk in the world measured in chunk sizes
    struct ChunkPos
    {
        union
        {
            vec3i Pos;
            struct
            {
                int x;
                int y;
                int z;
            };
        };

        ChunkPos() : x(0), y(0), z(0) { }

        explicit ChunkPos(vec3i pos)
        {
            Pos = pos;
        }

        ChunkPos(int x, int y, int z)
        {
            Pos.x = x;
            Pos.y = y;
            Pos.z = z;
        }

        vec3 ToWorldPos() const
        {
            return {
                x << 4,
                y << 4,
                z << 4,
            };
        }

        static ChunkPos FromWorldPos(vec3 worldPos)
        {
            // TODO: try to optimize this
            return {
                (int)std::floor(worldPos.x / 16.0f),
                (int)std::floor(worldPos.y / 16.0f),
                (int)std::floor(worldPos.z / 16.0f),
            };
        }
    };

    // Position of a block in a chunk (offset from the chunk position)
    struct BlockOffset
    {
        union
        {
            vec3b Pos;
            struct
            {
                byte x;
                byte y;
                byte z;
            };
        };

        BlockOffset() : x(0), y(0), z(0) { }

        explicit BlockOffset(vec3b pos)
        {
            Pos = pos;
        }

        BlockOffset(byte x, byte y, byte z)
        {
            Pos.x = x;
            Pos.y = y;
            Pos.z = z;
        }

        BlockPos ToBlockPos(const ChunkPos& chunkPos) const
        {
            return BlockPos((vec3i)chunkPos.ToWorldPos() + (vec3i)Pos);
        }

        static BlockOffset FromWorldPos(vec3 worldPos)
        {
            return BlockOffset(
                (byte)std::floor(worldPos.x) & 15,
                (byte)std::floor(worldPos.y) & 15,
                (byte)std::floor(worldPos.z) & 15
            );
        }
    };
}
