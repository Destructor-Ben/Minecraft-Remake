cmake_minimum_required(VERSION 3.15)
project(Minecraft_Remake VERSION 0.1)

# Adding GLAD library
add_library(
        glad
        lib/glad/glad.c
        lib/glad/include/glad/glad.h
        lib/glad/include/KHR/khrplatform.h
)

# Adding GLFW library
add_subdirectory(lib/glfw)

# Adding GLM library
add_subdirectory(lib/glm/glm)

# Include dirs
include_directories(lib/glad/include)
include_directories(lib/glfw/include)
include_directories(lib/glm)

# Main executable
add_executable(Minecraft_Remake
        src/Main.cpp
        src/Common.h
        src/Common.cpp
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
)

# Linking libraries
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
target_link_libraries(Minecraft_Remake glad)
