target_sources(Minecraft_Remake PRIVATE
    BiomeType.h
    Block.cpp
    Block.h
    BlockData.h
    BlockMeshType.h
    BlockType.h
    Chunk.cpp
    Chunk.h
    ChunkPriorityQueue.cpp
    ChunkPriorityQueue.h
    Coords.h
    World.cpp
    World.h
)

add_subdirectory(Biomes)
add_subdirectory(Blocks)
add_subdirectory(Generation)
