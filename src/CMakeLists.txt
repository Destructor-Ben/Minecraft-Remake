# Makes the binaries end up in build/[debug/release] instead of being in a src sub-folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Adding the source files
add_executable(Minecraft_Remake
    Common.h
    Game.cpp
    Game.h
    Hash.h
    LogManager.cpp
    LogManager.h
    Main.cpp
    Timers.h
    Transform.cpp
    Transform.h
    Window.h
)

add_subdirectory(Graphics)
add_subdirectory(Input)
add_subdirectory(Noise)
add_subdirectory(World)

# Auto include Common.h in all files
target_precompile_headers(Minecraft_Remake PRIVATE "Common.h")

# Allows includes from src e.g. #include "Game.h"
target_include_directories(Minecraft_Remake PRIVATE .)
