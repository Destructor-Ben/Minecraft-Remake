cmake_minimum_required(VERSION 3.22)
project(Minecraft_Remake VERSION 0.1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

# Main executable
add_executable(Minecraft_Remake assets/textures/application/icon.rc)
add_subdirectory(src)

# Source generators
add_subdirectory(generators/Version)

# Adding libraries
add_subdirectory(lib/cpptrace)
add_subdirectory(lib/fmt)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/stb_image_write)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm/glm)
add_subdirectory(lib/glfw)
add_subdirectory(lib/thread-pool)

target_include_directories(Minecraft_Remake PRIVATE lib/cpptrace/include)
target_include_directories(Minecraft_Remake PRIVATE lib/fmt/include)
target_include_directories(Minecraft_Remake PRIVATE lib/stb_image)
target_include_directories(Minecraft_Remake PRIVATE lib/stb_image_write)
target_include_directories(Minecraft_Remake PRIVATE lib/glad/include)
target_include_directories(Minecraft_Remake PRIVATE lib/glm)
target_include_directories(Minecraft_Remake PRIVATE lib/glfw/include)
target_include_directories(Minecraft_Remake PRIVATE lib/thread-pool/include)

target_link_libraries(Minecraft_Remake cpptrace::cpptrace)
target_link_libraries(Minecraft_Remake fmt)
target_link_libraries(Minecraft_Remake stb_image)
target_link_libraries(Minecraft_Remake stb_image_write)
target_link_libraries(Minecraft_Remake glad)
target_link_libraries(Minecraft_Remake glm)
target_link_libraries(Minecraft_Remake glfw ${GLFW_LIBRARIES})
