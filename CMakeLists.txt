cmake_minimum_required(VERSION 3.27)
project(Minecraft_Remake VERSION 0.1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

# Resources
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lang DESTINATION ${CMAKE_BINARY_DIR})

# Main executable
add_subdirectory(src)

# Source generators
add_subdirectory(generators/Version)

# Adding libraries
add_subdirectory(lib/stb_image)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm/glm)
add_subdirectory(lib/glfw)

target_include_directories(Minecraft_Remake PRIVATE lib/stb_image)
target_include_directories(Minecraft_Remake PRIVATE lib/glad/include)
target_include_directories(Minecraft_Remake PRIVATE lib/glm)
target_include_directories(Minecraft_Remake PRIVATE lib/glfw/include)

target_link_libraries(Minecraft_Remake stb_image)
target_link_libraries(Minecraft_Remake glad)
target_link_libraries(Minecraft_Remake glm)
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
