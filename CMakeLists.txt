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
add_subdirectory(src)

# Linking libraries
target_link_libraries(Minecraft_Remake stb_image)
target_link_libraries(Minecraft_Remake glad)
target_link_libraries(Minecraft_Remake glm)
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
