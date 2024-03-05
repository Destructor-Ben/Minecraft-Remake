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

    typedef signed char        int8;
    typedef signed short       int16;
    typedef signed int         int32;
    typedef signed long long   int64;
    typedef unsigned char      uint8;
    typedef unsigned short     uint16;
    typedef unsigned int       uint32;
    typedef unsigned long long uint64;

    inline void Log(const string& message)
    {
        std::cout << message << std::endl;
    }
}
