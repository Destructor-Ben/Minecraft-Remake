# To Do List

- Make the libraries submodules instead of directly embedded
- Statically link all libraries
- Embed resources - use source generator https://stackoverflow.com/a/71906177/12259381
- Transparency & blending
- Implement an ECS for entities and blocks
- Entities
- Make all chunk loops that use radii circle instead of squares
- The process segfaults on linux when exiting
- Maybe make chunk positions (origins) their center instead of corner
    - Blocks already are, also check that they are done correctly

## Optimizations

- Multithreading
    - Chunk meshing can be made to run in parallel
    - World generation can also be in parallel for certain passes, idk about stuff like tree placement, race conditions could affect things like that
        - No it wont, since chunks needs to generate independently anyway
    - Ticking and rendering should be separate processes? It's annoying when one blocks the same thread as the window
- Chunk and block pos getters are slow
    - Probably just calculate the positions in ctor then store, also just avoid function calls entirely
        - wastes memory to keep track of both a vector and its 3 components separately, so just store the vector
        - But we don't want them to be modifieable, so maybe keep the getters but make sure they have negligible impact
    - Make sure they are declared inline
    - Same with the util functions in World for positions
    - Also make GetChunk and GetBlock faster
- Transform::GetTransformMatrix is slow
    - TODO: figure out how to optimize
    - Cache the matrix?

### Rendering

- Chunk Meshing
    - TODO: investigate speed of chunk meshing
    - Cache the quaternions used in face calculation

### Updating

- Generating chunks around the player
    - Consider moving the remeshing queue to the renderer so it doesn't affect this function
