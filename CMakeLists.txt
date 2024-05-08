cmake_minimum_required(VERSION 3.27)
project(Minecraft_Remake VERSION 0.1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

# Adding compiler flags for gcc so this compiled on Linux
# Enables Microsoft extensions that allow certain variable naming conventions
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(GCC_COVERAGE_COMPILE_FLAGS "-fms-extensions")
    add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
endif ()

# Resources
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lang DESTINATION ${CMAKE_BINARY_DIR})

# Version source generator
# configure_file(generated/Version.cpp.in ${CMAKE_SOURCE_DIR}/generated/Version.cpp)
# list(APPEND MINECRAFT_REMAKE_GENERATED_FILES ${CMAKE_SOURCE_DIR}/generated/Version.cpp)

# Main executable
add_subdirectory(src)

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
