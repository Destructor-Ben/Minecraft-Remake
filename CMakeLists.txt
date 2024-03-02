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

# Resources
FILE(COPY res DESTINATION "${CMAKE_BINARY_DIR}")

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
)

# Linking libraries
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
target_link_libraries(Minecraft_Remake glad)
