#pragma once

#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL

// Common std includes
#include <any>
#include <chrono>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numbers>
#include <ranges>
#include <sstream>
#include <string>
#include <thread>
#include <variant>

// Collections
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

// Libraries
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

// Debug macro
#ifndef NDEBUG
#define DEBUG true
#else
#define DEBUG false
#endif

namespace Minecraft
{
    // Using std namespace aliases
    namespace chrono = std::chrono;
    namespace views = std::views;
    namespace numbers = std::numbers;

    // Strings
    typedef const char* cstring;

    using std::string;
    using std::to_string;
    using fmt::format;

    // Numbers
    typedef int8_t sbyte;
    // short
    // int
    typedef int64_t slong;

    typedef uint8_t byte;
    typedef uint16_t ushort;
    typedef uint32_t uint;
    typedef uint64_t ulong;

    // Vectors, matrices, and quaternions
    typedef glm::vec<2, int, glm::defaultp> vec2i;
    typedef glm::vec<3, int, glm::defaultp> vec3i;
    typedef glm::vec<4, int, glm::defaultp> vec4i;

    using glm::vec2;
    using glm::vec3;
    using glm::vec4;
    using glm::mat2;
    using glm::mat3;
    using glm::mat4;
    using glm::quat;

    // Pointers
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;

    using std::make_unique;
    using std::make_shared;

    // Optionals
    using std::optional;
    using std::nullopt;

    // Collections
    using std::array;
    using std::vector;
    using std::list;
    using std::stack;
    using std::deque;
    using std::queue;
    using std::priority_queue;
    using std::map;
    using std::set;
    using std::multimap;
    using std::multiset;
    using std::unordered_map;
    using std::unordered_set;
    using std::unordered_multimap;
    using std::unordered_multiset;
}
