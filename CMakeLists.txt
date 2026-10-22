cmake_minimum_required(VERSION 3.22)
project(Minecraft-Remake VERSION 0.1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

set(MINECRAFT_BUILD_TESTS ON)

# Makes the binaries end up in build/[debug/release] instead of being in a src sub-folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Core library
add_library(Minecraft-Core)
add_subdirectory(src)

target_include_directories(Minecraft-Core PUBLIC "src/")
target_precompile_headers(Minecraft-Core PUBLIC "src/Common.h")

# Dependencies
add_subdirectory(generators/Version)

add_subdirectory(lib/cpptrace)
add_subdirectory(lib/fmt)
add_subdirectory(lib/stb_image)
add_subdirectory(lib/stb_image_write)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm/glm)
add_subdirectory(lib/glfw)
add_subdirectory(lib/thread-pool)

target_include_directories(Minecraft-Core PUBLIC lib/cpptrace/include)
target_include_directories(Minecraft-Core PUBLIC lib/fmt/include)
target_include_directories(Minecraft-Core PUBLIC lib/glm)
target_include_directories(Minecraft-Core PUBLIC lib/glfw/include)
target_include_directories(Minecraft-Core PUBLIC lib/thread-pool/include)

target_link_libraries(Minecraft-Core PUBLIC cpptrace::cpptrace)
target_link_libraries(Minecraft-Core PUBLIC fmt)
target_link_libraries(Minecraft-Core PUBLIC stb_image)
target_link_libraries(Minecraft-Core PUBLIC stb_image_write)
target_link_libraries(Minecraft-Core PUBLIC glad)
target_link_libraries(Minecraft-Core PUBLIC glm)
target_link_libraries(Minecraft-Core PUBLIC glfw ${GLFW_LIBRARIES})
target_link_libraries(Minecraft-Core PUBLIC yaml-cpp::yaml-cpp)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.9.0
)
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the main executable and the tests exeutable
add_executable(Minecraft src/Main.cpp)
target_link_libraries(Minecraft PRIVATE Minecraft-Core)

if(WIN32)
  target_sources(Minecraft PRIVATE assets/textures/application/icon.rc)
endif()

if(MINECRAFT_BUILD_TESTS)
  add_executable(Minecraft-Tests)
  target_link_libraries(Minecraft-Tests PRIVATE Minecraft-Core)
  add_subdirectory(tests)
endif()
