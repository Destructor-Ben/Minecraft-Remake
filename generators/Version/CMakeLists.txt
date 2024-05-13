# Auto generates the version info
configure_file(Version.h.in Version.h)
target_include_directories(Minecraft_Remake PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND MINECRAFT_REMAKE_GENERATED_FILES Version.h)
