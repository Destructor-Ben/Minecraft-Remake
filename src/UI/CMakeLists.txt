target_sources(Minecraft_Remake PRIVATE
    UI.h
    UIDimension.cpp
    UIDimension.h
    UIElement.cpp
    UIElement.h
    UIState.cpp
    UIState.h
)

add_subdirectory(Elements)
add_subdirectory(States)
