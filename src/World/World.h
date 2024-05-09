#pragma once

#include "Common.h"
#include "Chunk.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"
#include "World/Generation/WorldGenerator.h"

namespace Minecraft
{
    class World
    {
    public:
        Camera Camera;
        // TODO: make this an unordered_map of Chunk
        /*
#pragma once

#include "src/Common.h"

namespace Minecraft
{
    // Implementation of an automatic hashing system for unordered_map
    // Based on https://stackoverflow.com/a/38140932/12259381
    inline void hash_combine(std::size_t & seed) { }

    template<typename T, typename... Rest>
    inline void hash_combine(std::size_t & seed, const T& v, Rest ... rest)
    {
        std::hash<T> hash;
        seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }
}

// Making types hashable
#define MINECRAFT_MAKE_HASHABLE(type, ...) \
    namespace std {\
        template<>\
        struct hash<type> {\
            size_t operator()(const type& t) const {\
                size_t hash = 0;\
                Minecraft::hash_combine(hash, __VA_ARGS__);\
                return hash;\
            }\
        };\
    }

MINECRAFT_MAKE_HASHABLE(Minecraft::vec3i, t.x, t.y, t.z);
         */
        //*/
        vector<Chunk> Chunks;

        World();
        ~World();

        void Tick();
        void Update();
        void Render();

        // Interface for chunks
        // TODO: getter for loaded chunks

        Block GetBlock(int32 x, int32 y, int32 z) { return GetBlock(vec3i(x, y, z)); }
        Block GetBlock(vec3i pos);

        // Mouse hiding
        bool IsMouseHidden() const { return m_IsMouseHidden; }
        void SetMouseHidden(bool hidden);

    private:
        void UpdateCamera();

        WorldGenerator m_WorldGenerator;

        bool m_IsMouseHidden = false;
    };
}
