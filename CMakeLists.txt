cmake_minimum_required(VERSION 3.15)
project(Minecraft_Remake VERSION 0.1)

add_library(
    glad
    ../src/glad.c
    ../include/glad/glad.h
    ../include/KHR/khrplatform.h
)

include_directories(include)

add_executable(main ../src/main.cpp)