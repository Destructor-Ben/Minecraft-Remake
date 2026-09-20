# Design

## Constraints

These were chosen so I could learn how to use these specific tools and programming styles. They are as follows:

- C++ 23
- CMake
- OpenGL 4.6
- Minimal libraries:
  - C++ utilities: `cpptrace`, `fmt`, `thread-pool`
  - Windowing: `glfw`
  - Rendering: `glad`
  - Maths: `glm`
  - Images: `stb_image`, `stb_image_write`

Note that `fmt` was included as a library when the project was started because of poor support for `std::fmt` in C++ 23.

## Programming Guidelines

These guidelines were decided on to ensure that my code is readable, testable, and easy to extend or optimize (such as through concurrency). These are only guidelines and may be broken if I feel they don't fit specific cirumstances. They are as follows:

- Test driven development for independent functionality, such as random number generation or noise functions.
- Avoid exceptions, since they obscure control flow.
- Avoid global variables, since they obscure data access. Ideally, only access function parameters.
  - Obscured data access makes refactoring and parallelizing difficult.
  - Global variables can also have issues with de-initializing smart pointers, which can cause segfaults when exiting the program.
<!-- TODO: explain this a bit better -->
- Avoid using manager classes/singletons for functionality that is better handled as a struct of state and namespaced functions that take that state as a parameter.
  - This ensures that it is still easy to make multiple instances of a manager if needed.
  - The main argument I hear for singletons is that they are easy to multiple instances of, since they are classes instead of global variables, except this point is moot if global variables are already avoided in the first place and handled as passed in state.
  - Manager classes force all private internals to be in a header file, which is frustrating for properly encapsulating functionality in headers and optimizing compile times since many compilation units get recompiled from a single header file changing an implementation detail.

<!-- TODO:
- folder structure of code
- add memory management tactics
-->

## Design Decisions

### Update Functions

The game uses 3 update functions:
- `Render()`
- `Update()`
- `Tick()`

`Tick()` should handle game logic, which runs at a fixed tick rate. `Update()` runs every frame before `Render()` and handles non-game logic, typically preparing and storing client state used in rendering, such as UI state or shader parameters. `Render()` runs every frame and should only make draw calls without modifying game state.

This was decided on because it makes it easy to extend the behaviour of the game in ways I may want to in the future, such as the following:
- Create a dedicated server: Remove `Update()` and `Render()` functions to only get game logic.
- Create a multiplayer client: Remove `Tick()` functions to get only client logic. The game state is then streamed from a server.
- Make a screenshot or screen recording tool: Store the game state/the changes to game state for a frame or multiple frames, then call `Render()` for each of those states without `Tick()` or `Update()` causing any state mutation.
- Pausing: Don't call `Tick()`, and ensure `Update()` knows that the game is paused.

<!--

- TODO: discuss more design decisions (these are the ones that I try to optimize the most)
- chunk data storage in both RAM and on disk
- data oriented entity-block data
- parallelization

-->
