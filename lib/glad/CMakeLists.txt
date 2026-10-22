add_library(
        glad
        glad.c
        include/glad/glad.h
        include/KHR/khrplatform.h
)

target_include_directories(glad PUBLIC include)
