#pragma once

#define GLFW_INCLUDE_NONE

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Minecraft
{
    typedef unsigned int uint;
    typedef std::string str;

    void Log(const str& message);
}
