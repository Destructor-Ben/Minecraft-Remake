# To Do List

- Worldgen border fixes + steps - look at how minecraft does it
- The process segfaults on linux when exiting
- Stars are broken on linux

- Partial transparency support for blocks
- Make all chunk loops that use radii circle instead of squares
- Make all materials automatically request their shaders? it makes no sense to make them not do it since it is passed in anyway

## Proper Design

- Internal server vs WorldAccess + syncing with TickThread?
  - Internal server allows opening to LAN + single codebase
  - Only separate thread is the server thread
  - If multithreading is desired, a threadpool should be used
    - Chunk meshing
    - Chunk generation
- World class rework
  - New coordinate system
    - WorldPos - vec3 (don't make a struct for this)
    - BlockPos - vec3i
    - ChunkPos - vec3i
    - BlockOffset - vec3i
  - Entities
  - Dimensions
  - NO biomes
  - Components
  - Faster coordinate calculations/conversions
  - Faster world access
  - Faster looping over blocks
  - Support for multithreading
- Renderer design
  - Lighting
    - Use minecraft style lightmap
      - Used for pointlights when they aren't set to deferred mode
      - Also used to calculate ambient light (from sun and moon), since caves should be darker than surface
    - Shadows from directional lights
    - Perhaps optiosn to use shadows for point lights
  - Deferred renderer - Almost all geometry
  - Forward - Partially transparent

## Build System

- Make the libraries submodules instead of directly embedded
- Statically link all libraries
- Make assets use an asset bundle

## Optimizations

- Possible memory leak
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

### Fix remeshing queue

- Fix holes in meshes when generating chunks around the player
- Generating chunks around the player
  - Consider moving the remeshing queue to the renderer so it doesn't affect this function
- World generation
  - Consider remeshing the world here, or possibly in a dedicated step, since otherwise it appears in the render function in profiles
- For the above two, its probably a good idea to just push chunks to a remeshing queue in the renderer
  - It makes more sense for it to be there
  - Also makes it easier to throttle the chunk meshing so only one mesh is updated per frame for example to prevent massive lagspikes
