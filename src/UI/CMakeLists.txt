target_sources(Minecraft_Remake PRIVATE
    UI.cpp
    UI.h
    UIElement.h
    UIState.cpp
    UIState.h
)

add_subdirectory(Elements)
add_subdirectory(States)
