# Minecraft Remake

A Minecraft remake in C++ and OpenGL.

## Building

The build system uses CMake (at least v3.22), along with some bash scripts for Linux in the `./scripts` folder to automate some of the build process and profiling. These are:
- `build.sh`: Build the game in release mode. Add the `debug` arg to build a debug version.
- `run.sh`: Run the release build of the game. Requires `build.sh` to have already been run. Add the `debug` arg to run the debug build.
- `run.sh`: Run the game with the `perf` profiler in release mode. Requires `build.sh` to have already been run. Add the `debug` arg to profile the debug build.
- `clean.sh`: Remove the `./bin` directory.

To build on windows, run `cmake -DCMAKE_BUILD_TYPE=Release -B bin/` and `cmake --build bin/` to build the game. To run it, run `./bin/Minecraft_Remake.exe`.

On NixOS, run `nix develop` to enter a devshell with the `./scripts` folder on `$PATH` and to install the required dependencies.

### Dependencies

TODO

## Design Constraints

These were chosen so I could learn how to use these specific tools and programming styles. They are as follows:

- C++ 23
- CMake
- OpenGL
- Minimal libraries:
  - C++ utilities: `cpptrace`, `fmt`, `thread-pool`
  - Windowing, rendering, maths: `glfw`, `glad`, and `glm`
  - Images: `stb_image`, `stb_image_write`
- Programming guidelines
    - Write tests for everything
    - Avoid global variables, since it makes it harder to figure out what a function changes and can be prone to shutdown bugs
    - Avoid objects and "manager classes" for things that are better as functions

Note that `fmt` is included as a library because of poor support for `std::fmt` in C++.

## Design Decisions

- TODO: discuss them (these are the ones that I try to optimize the most)
