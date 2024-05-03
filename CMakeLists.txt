cmake_minimum_required(VERSION 3.27)
project(Minecraft_Remake VERSION 0.1)
set(CMAKE_CXX_STANDARD 23)

# Adding libraries
add_subdirectory(lib/stb_image)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm/glm)
add_subdirectory(lib/glfw)

# Include dirs
include_directories(lib/stb_image)
include_directories(lib/glad/include)
include_directories(lib/glm)
include_directories(lib/glfw/include)

# Resources
FILE(COPY assets DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY data DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY lang DESTINATION "${CMAKE_BINARY_DIR}")

# Main executable
add_executable(Minecraft_Remake
        src/Main.cpp
        src/Common.h
        src/Game.h
        src/Game.cpp
        src/Graphics/FragmentShader.h
        src/Graphics/FragmentShader.cpp
        src/Graphics/VertexShader.h
        src/Graphics/VertexShader.cpp
        src/Graphics/Shader.h
        src/Graphics/Shader.cpp
        src/Graphics/VertexBuffer.h
        src/Graphics/VertexBuffer.cpp
        src/Graphics/IndexBuffer.h
        src/Graphics/IndexBuffer.cpp
        src/Input/InputManager.h
        src/Input/InputManager.cpp
        src/Input/Key.h
        src/Graphics/VertexArray.cpp
        src/Graphics/VertexArray.h
        src/Graphics/Window.h
        src/Graphics/Window.cpp
        src/Graphics/Renderer.cpp
        src/Graphics/Renderer.h
        src/Input/MouseButton.h
        src/Time.cpp
        src/Time.h
        src/Graphics/Material.cpp
        src/Graphics/Material.h
        src/Graphics/Mesh.cpp
        src/Graphics/Mesh.h
        src/World/World.cpp
        src/World/World.h
        src/World/Chunk.cpp
        src/World/Chunk.h
        src/World/Block.cpp
        src/World/Block.h
        src/World/Entity.cpp
        src/World/Entity.h
        src/Graphics/Camera.cpp
        src/Graphics/Camera.h
        src/World/Transform.cpp
        src/World/Transform.h
        src/World/BlockType.h
        src/World/WorldGenerator.cpp
        src/World/WorldGenerator.h
        src/World/BlockData.cpp
        src/World/BlockData.h
        src/Graphics/Texture.cpp
        src/Graphics/Texture.h
        src/Graphics/Quad.h
        src/Graphics/ChunkRenderer.cpp
        src/Graphics/ChunkRenderer.h
        src/Graphics/GraphicsResource.h
        src/Logger.cpp
        src/Logger.h
        src/Graphics/Vertex.h
        src/Graphics/Quad.cpp
        src/Noise/Perlin.cpp
        src/Noise/Perlin.h
        src/Noise/Noise.cpp
        src/Noise/Noise.h
)

# Linking libraries
target_link_libraries(Minecraft_Remake stb_image)
target_link_libraries(Minecraft_Remake glad)
target_link_libraries(Minecraft_Remake glm)
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
