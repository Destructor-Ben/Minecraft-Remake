target_sources(Minecraft-Core PRIVATE
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
