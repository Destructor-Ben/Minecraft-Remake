# Auto generates the version info
configure_file(Version.h.in Version.h)

target_include_directories(Minecraft-Core PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_sources(Minecraft-Core PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Version.h)
