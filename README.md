# Minecraft Remake

A Minecraft remake in C++ and OpenGL.

## Building

The build system uses CMake (at least v3.22), along with some bash scripts for Linux in the `./scripts` folder to automate some of the build process and profiling. These are:
- `build.sh`: Build the game in release mode. Add the `debug` arg to build a debug version.
- `run.sh`: Run the release build of the game. Requires `build.sh` to have already been run. Add the `debug` arg to run the debug build.
- `clean.sh`: Remove the `./bin` directory.
- `profile.sh`: Run the game with `perf` in release mode. Requires `build.sh` to have already been run. Add the `debug` arg to profile the debug build.
- `mem-test.sh`: Run the game with `valgrind` in release mode. Requires `build.sh` to have already been run. Add the `debug` arg to test the debug build.
- `debugger.sh`: Run the game with `gdb` in debug mode. Requires `build.sh` to have already been run. Does not work in release mode.

To build on Windows, run `cmake -DCMAKE_BUILD_TYPE=Release -B bin/` and `cmake --build bin/` to build the game. To run it, run `./bin/Minecraft_Remake.exe`.

On NixOS, run `nix develop` to enter a devshell with the `./scripts` folder on `$PATH` and to install the required dependencies.

### Dependencies

No dependencies should be needed to be installed to build or run this, since all used libraries are either typically installed by default or packaged with the source code.

However, if you do have issues with dependencies on Linux, please take a look at [`flake.nix`](./flake.nix) lines 22 to 50 to see if any of the listed dependencies must be installed.

## Generative AI Usage Disclaimer

Originally, generative AI was used sparingly throughout this project while I was learning C++ and OpenGL for algorithms I didn't understand. For example. the original code for the Amantides-Woo voxel traversal algorithm, camera frustum bounds check, perlin noise generator, and random number generator had parts written by AI. However, as I learned more, I realized that generative AI wasn't effective at getting me to learn (because it was doing the work for me), and was sometimes very misleading. Because of this, generative AI is not used for this project as of [this](https://github.com/Destructor-Ben/Minecraft-Remake/commit/e467fa452733a8ffbcacf431109cb16d461fb0eb) commit, and any usage of AI written code that I find in this project will be rewritten if I find it to ensure I actually understand the concepts and implementation details that I failed to learn because of AI usage.

## Design Constraints

These were chosen so I could learn how to use these specific tools and programming styles. They are as follows:

- C++ 23
- CMake
- OpenGL
- Minimal libraries:
  - C++ utilities: `cpptrace`, `fmt`, `thread-pool`
  - Windowing, rendering, maths: `glfw`, `glad`, and `glm`
  - Images: `stb_image`, `stb_image_write`

Note that `fmt` is included as a library because of poor support for `std::fmt` in C++.

## Programming Guidelines

These guidelines were decided on to ensure that my code is readable, testable, and easy to extend or optimize (such as through concurrency). These are only guidelines and may be broken if I feel they don't fit specific cirumstances. They are as follows:

- Write independent tests for functionality that can be tested independently from the game, such as random number generation or noise functions.
- Avoid global variables, since it isn't clear what variables a function accesses if they are used. Ideally, only access function parameters.
  - Global variables also have issues with de-initializing smart pointers, which can cause segfaults when exiting the program.
  - They also make parallelization harder because they obscure what data a function accesses.
- Avoid using manager classes/singletons for functionality that is better handled as a struct of state and namespaced functions that take that state as a parameter.
  - This ensures that it is still easy to make multiple instances of a manager if needed.
  - The main argument I hear for singletons is that they are easy to multiple instances of, since they are classes instead of global variables, except this point is moot if global variables are already avoided in the first place and handled as passed in state.
  - Manager classes force all private internals to be in a header file, which is frustrating for properly encapsulating functionality in headers and optimizing compile times since many compilation units get recompiled from a single header file changing an implementation detail.

<!-- TODO: folder structure of code,
  - add memory management tactics
  - add modularity tactics
   -->

## Design Decisions

### Update Functions

The game uses 3 update functions:
- `Render()`
- `Update()`
- `Tick()`

Tick should handle game logic, which runs at a fixed tick rate. Update runs every frame before Render and handles non-game logic, typically preparing client state used in rendering, such as UI state or updating shader settings. Render runs every frame and should only make draw calls without modifying game state.

This was decided on because it makes it easy to extend the behaviour of the game in ways I may want to in the future, such as bythe following:
- Create a dedicated server: Remove Update and Render functions to only get game logic.
- Create a multiplayer client: Remove Tick functions to get only client logic. The game state is then streamed from a server.
- Make a screenshot or screen recording tool: Store the game state/the changes to game state for a frame or multiple frames, then call Render for each of those states without Tick or Update causing any state mutation.
- Pausing: Don't call Tick, and ensure Update knows that the game is paused.

<!--

- TODO: discuss more design decisions (these are the ones that I try to optimize the most)
- chunk data storage in both RAM and on disk
- data oriented entity-block data
- parallelization

-->
