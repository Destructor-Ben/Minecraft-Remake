# To Do List

- Fix compiling for Oliver
    - Make a placeholder for `format()` since he doesn't have the correct library for it

```cpp
template<typename... _Args>
inline string format(string __fmt, _Args&& ... __args) { return "[FORMAT PLACEHOLDER]"; }
```

- Also fix `stacktrace` being nonexistent for me and him

- Make the libraries submodules instead of directly embedded
- Embed resources - use source generator https://stackoverflow.com/a/71906177/12259381
- Transparency & blending
- Implement an ECS for entities and blocks
- Entities
- Make all chunk loops that use radii circle instead of squares
- Maybe make chunk positions (origins) their center instead of corner
    - Blocks already are, also check that they are done correctly
- Why is it so laggy when under the world?

## Optimizations

- Multithreading
    - Chunk meshing can be made to run in parallel
    - World generation can also be in parallel for certain passes, idk about stuff like tree placement, race conditions could affect things like that
    - Ticking and rendering should be separate processes? It's annoying when one blocks the same thread as the window
- Chunk and block pos getters are slow
    - Probably just calculate the positions in ctor then store, also just avoid function calls entirely
        - wastes memory to keep track of both a vector and its 3 components separately, so just store the vector
        - But we don't want them to be modifieable, so maybe keep the getters but make sure they have negligible impact
    - Make sure they are declared inline
    - Same with the util functions in World for positions
    - Also make GetChunk and GetBlock faster
- Transform::GetTransformMatrix is slow

### Loading

- Loading the skybox is very slow, maybe downsize it?
    - Could remove it entirely, it is just a placeholder

### Rendering

- CameraFrustum class is very slow
    - Probably cache the frustum instead of retrieving it every draw call
        - Do similarly to how the view and projection matrices are obtained
    - Allow CameraFrustum::GetFrustum to take in the matrices to avoid recalculating them
- Chunk Meshing is horribly slow
    - TODO: investigate further

### Updating

- Generating chunks around the player
    - Slow because of the chunk meshing
    - TODO: investigate why so much chunk remeshing happens while underneath the world?
    - TODO: consider moving the remeshing queue to the renderer so it doesn't affect this function
