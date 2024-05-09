# Auto generates the version info
configure_file(Version.cpp.in Version.cpp)

add_library(Version STATIC Version.cpp)
target_link_libraries(Minecraft_Remake Version)
