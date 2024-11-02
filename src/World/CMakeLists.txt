target_sources(Minecraft_Remake PRIVATE
    Block.cpp
    Block.h
    BlockData.h
    BlockType.h
    Chunk.cpp
    Chunk.h
    World.cpp
    World.h
)

add_subdirectory(Generation)
