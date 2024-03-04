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
    typedef unsigned int uint;
    typedef std::string str;

    inline void Log(const str& message)
    {
        std::cout << message << std::endl;
    }

    template<typename T>
    inline str ToStr(const T& value)
    {
        return std::to_string(value);
    }
}
