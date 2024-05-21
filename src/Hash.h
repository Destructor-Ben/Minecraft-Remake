#pragma once

#include "Common.h"

// Implementation of an automatic hashing system for unordered_map
// Based on https://stackoverflow.com/a/38140932/12259381

namespace Minecraft
{
    inline void HashCombine(std::size_t & seed) { }

    template<typename T, typename... Rest>
    inline void HashCombine(std::size_t & seed, const T& v, Rest ... rest)
    {
        std::hash<T> hash;
        seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        HashCombine(seed, rest...);
    }

    // TODO: temporary, can't get hashing for vec3i to work well
    struct ChunkKeyHash
    {
        std::size_t operator ()(const vec3i& t) const
        {
            size_t hash = 0;
            HashCombine(hash, t.x, t.y, t.z);
            return hash;
        }
    };
}

/* TODO: get hashes working again
// Making types hashable
#define MINECRAFT_MAKE_HASHABLE(type, ...) \
    template<>\
    struct std::hash<type> {\
        size_t operator()(const type& t) const {\
            size_t hash = 0;\
            Minecraft::HashCombine(hash, __VA_ARGS__);\
            return hash;\
        }\
    };

//MINECRAFT_MAKE_HASHABLE(Minecraft::vec3i, t.x, t.y, t.z)
*/
