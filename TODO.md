# To Do List

- Fix holes in meshes when generating chunks around the player
- Worldgen border fixes + steps - look at how minecraft does it
- The process segfaults on linux when exiting

- Partial transparency support for blocks
- Make all chunk loops that use radii circle instead of squares
- Make all materials automatically request their shaders? it makes no sense to make them not do it since it is passed in anyway

## UI

- The UI code is absolutely cooked
    - Bounds calculations looks correct
    - Need to rewrite the lowest level stuff first then higher level stuff
    - Sprites
        - Sprite.FromRect and Sprite.Origin need checking for weird behaviour
        - Test thoroughly
    - Text
        - Make sure all the parameters work properly
        - Test thoroughly
    - Go through UIElement and UIState and make sure all of the overrides are covered
        - OnResize
        - Ensure the base function is being called in the correct order
    - Now rewrite the basic elements
    - Now rewrite the UI states
    - Once that is done, checking that origins work correctly when using the same origin across multiple sprites
        - Text rendering
        - Button rendering
    - Make sure includes are good
- Finish UI states
    - Main menu
    - Pause menu
    - Debug UI
    - Crosshair
    - Open UI fade (the black tint)
    - Hotbar
    - Inventory
- UI Elements
    - UIElement
    - UISprite
    - UIText
    - UIButton
    - All the origins are cooked - Check Sprite.SetTargetRect

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
