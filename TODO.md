# To Do List

- Worldgen border fixes + steps - look at how minecraft does it
- The process segfaults on linux when exiting
- Make visualizers for bounds objects and also points
    - Also a wireframe mode
- Fix chunk meshing speed
    - Redo coordinate stuff
        - ChunkOffset
        - BlockOffset
        - BlockPos
        - WorldPos
        - Maybe make chunk positions (origins) their center instead of corner
            - Blocks already are, also check that they are done correctly

- Partial transparency support for blocks
- Entities
- ECS for entities and blocks
- Make all chunk loops that use radii circle instead of squares
- Finish the sky renderer
- Cloud renderer

## Build System

- Make the libraries submodules instead of directly embedded
- Statically link all libraries
- Make assets use an asset bundle

## Optimizations

- What exactly changes in release builds that improves performance so much?
- Debug + Release options for linux, since inlining happens with the release build which prevents profiling effectively (I assume this is the issue)

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

### Updating

- Generating chunks around the player
    - Consider moving the remeshing queue to the renderer so it doesn't affect this function
- World generation
    - Consider remeshing the world here, or possibly in a dedicated step, since otherwise it appears in the render function in profiles
- For the above two, its probably a good idea to just push chunks to a remeshing queue in the renderer
    - It makes more sense for it to be there
    - Also makes it easier to throttle the chunk meshing so only one mesh is updated per frame for example to prevent massive lagspikes
