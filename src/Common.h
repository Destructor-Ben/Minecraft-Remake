#pragma once

#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <string>
#include <format>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <any>
#include <variant>
#include <memory>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <unordered_map>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stb_image.h>

#ifndef NDEBUG
#define DEBUG true
#else
#define DEBUG false
#endif

namespace Minecraft
{
    // Strings
    typedef const char* cstring;

    using std::string;
    using std::to_string;
    using std::format;

    // Numbers
    typedef float float32;
    typedef double float64;

    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    // Vectors, matrices, and quaternions
    typedef glm::vec<2, int32, glm::defaultp> vec2i;
    typedef glm::vec<3, int32, glm::defaultp> vec3i;
    typedef glm::vec<4, int32, glm::defaultp> vec4i;

    using glm::mat4;
    using glm::quat;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    // Pointers
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;

    using std::make_unique;
    using std::make_shared;

    // TODO: using std::vetor
}
