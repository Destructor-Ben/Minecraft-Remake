// Preprocessor directives order:
// - .cpp or .h
//   - .h: #pragma once
//   - .h/Main.cpp: Common.h
//   - .cpp: Corresponding header file
// - Regular imports
//   - Own files ("")
//   - Std lib (<>)
//   - Other libs (<>)
// - #defines

#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <vector>
#include <list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Minecraft
{
    using std::string;
    using std::to_string;

    using glm::mat4;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    inline void Log(const string& message)
    {
        std::cout << message << std::endl;
    }
}
