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

# Include dirs
include_directories(lib/glad/include)
include_directories(lib/glfw/include)

# Main executable
add_executable(main src/Main.cpp)

# Linking libraries
target_link_libraries(main glfw ${GLFW_LIBRARIES})
target_link_libraries(main glad)
