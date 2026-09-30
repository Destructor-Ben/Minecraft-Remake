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

These general programming guidelines were decided on to ensure that my code is readable, testable, safe, and extensible. These are only guidelines and may be broken if I feel they don't work in specific cirumstances. They are as follows:

- Test driven development for independent functionality, such as random number generation or noise functions.
- Avoid exceptions, since they obscure control flow and can easily cause memory leaks. Use optionals instead.
- Avoid `nullptr` for 'empty' variables. Use optionals instead to guarantee safety.
- Avoid `new` and `delete`: Use smart pointers to guarantee safety.
- Avoid inheritance: It is usually clearer what a class implements itself by including the parent class as a field, forcing explicit access (e.g. `Entity.Transform` instead of `class Entity : public Transform`).
- Avoid global variables because they obscure data access. Ideally, only access function parameters:
  - Obscured data access makes refactoring and parallelizing difficult.
  - Global variables can also have issues with smart pointers destructors, which can cause segfaults after the main function returns.
  - Occassionally, global variables can be used for data that only requires one instance (such as a log file handle) and is kept hidden from most of the code to avoid polluting it.

## Design Decisions

These specific design decisions were chosen to make specifically a game or engine easy to use.

### Decouple State and Functionality

Decouples state is typically implemented by creating state (plain structs) and systems (functions receiving the state) in `src/World/State/` and `src/World/Systems/`.

A few requirements are needed for this decoupling to work:
- State must NOT store pointers: Storing pointers makes it possible for invalid pointers to exist in state. If a pointer to state is needed, there are ways to handle that (such as an entity ID or a block pos) that can be easily validated.
- All state should be public: There is no reason to make it private and it makes changing variable accessibility (and following public/private naming conventions) harder when changes are needed.
- Systems should (ideally) validate their input: This is obviously a good idea because it makes errors more difficult to occur from minor changes. It is also very important because of the above point.

This is important for these reasons:
- Clear data flow: It is easier to see what code accesses what state. This makes it much easier to debug issues with unexpected state.
- Separation of concerns (storing state vs using state), making it easy to separate the update functions (discussed below), as well as serialization and deserialization.
- Multithreading: It is hard to safely multithread functions when data flow is obscured (discussed further below).
- (Lack of) encapsulation: Encapsulation is a good pattern for a library to guarantee a stable interface, but this is a game/engine, not a library. Encapsulation typically makes data access difficult when plans and ideas change, which is common for games. Obscuring data access is also only necessary if you can have invalid state, another purpose of encapsulating is to prevent tampering.

Note that I am not using an ECS to manage state and systems. This is because I didn't want to rely on a library for one (for learning experience) and I didn't want to create one from scratch, since it is a huge workload.

### Update Functions

The game uses 3 update functions:
- `Render()`
- `Update()`
- `Tick()`

`Tick()` should handle game logic, which runs at a fixed tick rate. `Update()` runs every frame before `Render()` and handles non-game logic, typically preparing and storing client state used in rendering, such as UI state or shader parameters. `Render()` runs every frame and should only make draw calls without modifying game state.

This was decided on because it makes it easy to extend the behaviour of the game in ways I may want to in the future, such as the following:
- Create a dedicated server: Remove `Update()` and `Render()` to only get game logic.
- Create a multiplayer client: Remove `Tick()` to get only client logic. The game state can then be streamed from a server.
- Make a screenshot or screen recording tool: Store the game state/the changes to game state for a frame or multiple frames, then call `Render()` for each of those states without `Tick()` or `Update()` causing any state mutation.
- Pausing: Don't call `Tick()`, and ensure `Update()` knows that the game is paused.

### Multithreading

WIP.

<!--

- TODO: discuss:
- chunk data storage in both RAM and on disk
- parallelization
  - include RAII mutexes/rwlocks that return the data that they access
- TODO: RAII OpenGL resources (make sure they have removed copy ctor)
- backend agnostic renderer (if that happens)

-->
