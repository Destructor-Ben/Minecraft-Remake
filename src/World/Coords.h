#pragma once

namespace Minecraft
{
    // World pos is a generic floating point position in the world
    // All the conversions assume chunk sizes are 16 (since they are optimized for sizes that are powers of 2)
    // Unions are used to make accessing x, y, z, and pos easy
    // Old impl sucked because it didn't use floor and instead cast to int and then adjusted for negatives
    // Since C++ rounds to 0 when casting to int, when I need rounding to negative infinity.

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
        explicit BlockPos(vec3i pos) : Pos(pos) { }
        BlockPos(int x, int y, int z) : Pos(x, y, z) { }

        vec3 ToWorldPos() const
        {
            return vec3(Pos);
        }

        static BlockPos FromWorldPos(float x, float y, float z) { return FromWorldPos(vec3(x, y, z)); }
        static BlockPos FromWorldPos(vec3 worldPos)
        {
            return BlockPos(glm::floor(worldPos));
        }

        bool operator ==(const BlockPos& rhs) const
        {
            return (Pos == rhs.Pos);
        }

        bool operator !=(const BlockPos& rhs) const
        {
            return !operator ==(rhs);
        }

        string to_string() const
        {
            return format("BlockPos({}, {}, {})", x, y, z);
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
        explicit ChunkPos(vec3i pos) : Pos(pos) { }
        ChunkPos(int x, int y, int z) : Pos(x, y, z) { }

        vec3 ToWorldPos() const
        {
            return {
                x << 4,
                y << 4,
                z << 4,
            };
        }

        static ChunkPos FromWorldPos(float x, float y, float z) { return FromWorldPos(vec3(x, y, z)); }
        static ChunkPos FromWorldPos(vec3 worldPos)
        {
            return FromBlockPos(BlockPos::FromWorldPos(worldPos));
        }

        static ChunkPos FromBlockPos(int x, int y, int z) { return FromBlockPos(BlockPos(x, y, z)); }
        static ChunkPos FromBlockPos(const BlockPos& blockPos)
        {
            return {
                blockPos.x >> 4,
                blockPos.y >> 4,
                blockPos.z >> 4,
            };
        }

        bool operator ==(const ChunkPos& rhs) const
        {
            return (Pos == rhs.Pos);
        }

        bool operator !=(const ChunkPos& rhs) const
        {
            return !operator ==(rhs);
        }

        string to_string() const
        {
            return format("ChunkPos({}, {}, {})", x, y, z);
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
        explicit BlockOffset(vec3b pos) : Pos(pos) { }
        BlockOffset(byte x, byte y, byte z) : Pos(x, y, z) { }

        BlockPos ToBlockPos(const ChunkPos& chunkPos) const
        {
            return BlockPos((vec3i)chunkPos.ToWorldPos() + (vec3i)Pos);
        }

        static BlockOffset FromWorldPos(float x, float y, float z) { return FromWorldPos(vec3(x, y, z)); }
        static BlockOffset FromWorldPos(vec3 worldPos)
        {
            return FromBlockPos(BlockPos::FromWorldPos(worldPos));
        }

        static BlockOffset FromBlockPos(int x, int y, int z) { return FromBlockPos(BlockPos(x, y, z)); }
        static BlockOffset FromBlockPos(const BlockPos& blockPos)
        {
            return BlockOffset(
                (byte)(blockPos.x & 15),
                (byte)(blockPos.y & 15),
                (byte)(blockPos.z & 15)
            );
        }

        bool operator ==(const BlockOffset& rhs) const
        {
            return (Pos == rhs.Pos);
        }

        bool operator !=(const BlockOffset& rhs) const
        {
            return !operator ==(rhs);
        }

        string to_string() const
        {
            return format("BlockOffset({}, {}, {})", x, y, z);
        }
    };

    inline std::ostream& operator <<(std::ostream& os, const BlockPos& pos)
    {
        os << pos.to_string();
        return os;
    }

    inline std::ostream& operator <<(std::ostream& os, const ChunkPos& pos)
    {
        os << pos.to_string();
        return os;
    }

    inline std::ostream& operator <<(std::ostream& os, const BlockOffset& pos)
    {
        os << pos.to_string();
        return os;
    }

    inline string to_string(const BlockPos& pos)
    {
        return pos.to_string();
    }

    inline string to_string(const ChunkPos& pos)
    {
        return pos.to_string();
    }

    inline string to_string(const BlockOffset& pos)
    {
        return pos.to_string();
    }
}
