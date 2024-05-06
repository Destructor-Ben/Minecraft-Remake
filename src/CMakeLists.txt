# Makes the binaries end up in build/[debug/release] instead of being in a src sub-folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Adding the source files
add_executable(Minecraft_Remake
        ${MINECRAFT_REMAKE_GENERATED_FILES}
        Main.cpp
        Common.h
        Game.h
        Game.cpp
        Graphics/FragmentShader.h
        Graphics/FragmentShader.cpp
        Graphics/VertexShader.h
        Graphics/VertexShader.cpp
        Graphics/Shader.h
        Graphics/Shader.cpp
        Graphics/VertexBuffer.h
        Graphics/VertexBuffer.cpp
        Graphics/IndexBuffer.h
        Graphics/IndexBuffer.cpp
        Input/InputManager.h
        Input/InputManager.cpp
        Input/Key.h
        Graphics/VertexArray.cpp
        Graphics/VertexArray.h
        Graphics/Renderer.cpp
        Graphics/Renderer.h
        Input/MouseButton.h
        Graphics/Material.cpp
        Graphics/Material.h
        Graphics/Mesh.cpp
        Graphics/Mesh.h
        World/World.cpp
        World/World.h
        World/Chunk.cpp
        World/Chunk.h
        World/Block.cpp
        World/Block.h
        World/Entity.cpp
        World/Entity.h
        Graphics/Camera.cpp
        Graphics/Camera.h
        World/Transform.cpp
        World/Transform.h
        World/BlockType.h
        World/WorldGenerator.cpp
        World/WorldGenerator.h
        World/BlockData.cpp
        World/BlockData.h
        Graphics/Texture.cpp
        Graphics/Texture.h
        Graphics/Quad.h
        Graphics/ChunkRenderer.cpp
        Graphics/ChunkRenderer.h
        Graphics/GraphicsResource.h
        LogManager.cpp
        LogManager.h
        Graphics/Vertex.h
        Graphics/Quad.cpp
        Noise/Perlin.cpp
        Noise/Perlin.h
        Noise/Noise.cpp
        Noise/Noise.h
)

# Allows includes from src e.g. #include "Common.h"
target_include_directories(Minecraft_Remake PRIVATE .)
