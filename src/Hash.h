#pragma once

// Implementation of an automatic hashing system for unordered_map
// Based on https://stackoverflow.com/a/38140932/12259381

namespace Minecraft
{
    inline void HashCombine(std::size_t& seed) { }

    template<typename T, typename... Rest>
    inline void HashCombine(std::size_t& seed, const T& v, Rest ... rest)
    {
        std::hash<T> hash;
        seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        HashCombine(seed, rest...);
    }

    template<typename... Values>
    inline ulong HashMultiple(Values ... values)
    {
        size_t hash = 0;
        HashCombine(hash, values...);
        return hash;
    }

    template<typename T>
    inline ulong Hash(T value)
    {
        return std::hash<T> { }(value);
    }
}

// Making types hashable
#define MINECRAFT_MAKE_HASHABLE(type, ...) \
    template<>\
    struct std::hash<type> {\
        size_t operator()(const type& t) const { \
            return Minecraft::HashMultiple(__VA_ARGS__);\
        }\
    };

MINECRAFT_MAKE_HASHABLE(Minecraft::vec3i, t.x, t.y, t.z)
