# Minecraft Remake

A Minecraft remake in C++ 23 and OpenGL.

## Dependencies

The following will need to be installed to build the game:
- CMake (minimum version 3.22)
- A C++ compiler with C++ 23 support (just install Visual Studio on Windows)

There are no runtime dependencies to install.

## Building

### Windows

```pwsh
git clone "https://github.com/Destructor-Ben/Minecraft-Remake"
cd "Minecraft-Remake"

cmake -B build/
cmake --build build/ --config Release

cd build/Release
./Minecraft_Remake.exe
```

### Linux

```bash
git clone "https://github.com/Destructor-Ben/Minecraft-Remake"
cd "Minecraft-Remake"

cmake -DCMAKE_BUILD_TYPE=Release -B build/
cmake --build build/

./build/Minecraft_Remake
```

### MacOS

Untested, but likely the same as the Linux build instructions.

## Controls

WASD controls horizontal movement, Shift and Space control vertical movement.

The grave/tilde key toggles the debug overlay. Escape opens the pause menu.

Left click breaks blocks, right click places blocks. 1 through 9 select different blocks to place, 0 deselects the block to place.

## Generative AI Usage Disclaimer

Originally, generative AI was used sparingly throughout this project while I was learning C++ and OpenGL. For example. the original code for the Amantides-Woo voxel traversal algorithm, camera frustum bounds check, perlin noise generator, and random number generator had parts written by AI.

As I learned more, I realized that generative AI wasn't effective at getting me to learn. Because of this, it is guaranteed that from
[this commit](https://github.com/Destructor-Ben/Minecraft-Remake/commit/e467fa452733a8ffbcacf431109cb16d461fb0eb)
onwards, no new code will be written by generative AI and existing AI written code will be rewritten if I find it.

## Design

Please read [this](./DESIGN.md) if you want to read about the design of this project.
