include_directories(include)

add_library(
        glad
        glad.c
        include/glad/glad.h
        include/KHR/khrplatform.h
)
