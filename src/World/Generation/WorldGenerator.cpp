#include "WorldGenerator.h"

#include <thread>
#include <thread_pool/thread_pool.h>

#include "Game.h"
#include "Logger.h"
#include "Profiler.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "World/World.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, ulong seed) :
        m_World(world),
        m_Seed(seed),
        m_Noise(seed)
    {
        // TODO: should this be moved to below? any long init things should happen on the worldgen/tick thread in future
        InitBiomeMap();
        InitSurfaceBlocksMap();
    }

    void WorldGenerator::Generate(int spawnRadius, int minHeight, int maxHeight)
    {
        Instance->PerfProfiler->Push("WorldGenerator::Generate");

        // TODO: properly choose threadpool size
        dp::thread_pool chunkGenThreadPool(8);
        std::mutex chunkMutex;
        std::mutex meshRegenMutex;

        for_chunk_in_radius_2D(x, z, spawnRadius, {
            for (int y = minHeight; y <= maxHeight; ++y)
            {
                chunkGenThreadPool.enqueue_detach(
                    [this, x, y, z, &chunkMutex, &meshRegenMutex]()
                    {
                        // Create and generate the chunk
                        auto pos = ChunkPos(x, y, z);

                        // TODO: make the regular function threadsafe
                        Chunk* chunk = nullptr;

                        {
                            std::lock_guard lock(chunkMutex);
                            m_World->Chunks.emplace(pos, Chunk(pos));
                            chunk = &m_World->Chunks.at(pos);
                        }

                        Generate(*chunk);

                        // Remesh the chunk
                        {
                            std::lock_guard lock(meshRegenMutex);
                            // TODO: priority
                            Instance->ChunkGraphics->QueueMeshRegen(*chunk);
                        }
                    }
                );
            }
        })

        // TODO: in future, don't wait and run while the game updates
        // - Perhaps at a certain point each frame, check the status of each task and if some are done,
        //   take the finished chunks and add them, and the unfinished chunks can continue to be worked on.
        //   Rinse and repeat until all of the chunks are generated.
        chunkGenThreadPool.wait_for_tasks();

        Instance->PerfProfiler->Pop();
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight)
    {
        Instance->PerfProfiler->Push("WorldGenerator::GenerateChunksAroundPlayer");

        // Only generate new chunks when moving along chunk borders
        if (!m_World->HasPlayerMovedChunks)
        {
            // TODO: just don't start the profiler unless the player has moved chunks
            // TODO: repeat that pattern for other functions
            Instance->PerfProfiler->Pop();
            return;
        }

        // TODO: properly choose threadpool size
        // TODO: don't recreate the threadpool, reuse an existing one
        // TODO-IMPORTANT: COMPARE MULTITHREADED + SINGLE THREADED PERF =================================================
        dp::thread_pool chunkGenThreadPool(8);
        std::mutex chunkMutex;
        std::mutex meshRegenMutex;

        auto playerChunkPos = ChunkPos::FromWorldPos(playerPos);

        // TODO: what the fuck why isn't this working!!
        for_chunk_in_radius(x, y, z, radius, {
            // TODO: maybe multithread this part? [[
            // Calculate chunk pos
            auto chunkPos = ChunkPos(x, y, z);
            chunkPos.Pos += playerChunkPos.Pos;

            // Check if the y position is out of bounds
            if (chunkPos.y < minHeight || chunkPos.y > maxHeight)
                continue;
            // TODO END ]]

            chunkGenThreadPool.enqueue_detach(
                [this, chunkPos, &chunkMutex, &meshRegenMutex]()
                {
                    // Check if the chunk exists and create it
                    Chunk* chunk = nullptr;

                    // TODO: use a function
                    // TEMP - need a lock here, handle this better in the future
                    {
                        std::lock_guard lock(chunkMutex);

                        auto existingChunk = m_World->GetChunk(chunkPos);
                        if (existingChunk.has_value())
                            return;

                        m_World->Chunks.emplace(chunkPos, Chunk(chunkPos));
                        chunk = &m_World->Chunks.at(chunkPos);
                    }

                    // Generate the chunk
                    Generate(*chunk);

                    // Regenerate chunk meshes
                    // m_ChunkRemeshDirections includes the current chunk and all neighbors
                    {
                        std::lock_guard lock(meshRegenMutex);

                        for (const auto& direction : m_ChunkRemeshDirections)
                        {
                            auto otherChunkPos = chunkPos;
                            otherChunkPos.Pos += direction;
                            auto chunkToRemesh = m_World->GetChunk(otherChunkPos); // TODO: maybe this line should have a lock
                            if (chunkToRemesh.has_value())
                            {
                                // TODO: priority
                                Instance->ChunkGraphics->QueueMeshRegen(*chunkToRemesh.value());
                            }
                        }
                    }
                }
            );
        })

        // TODO: read the above threadpool impl
        chunkGenThreadPool.wait_for_tasks();

        Instance->PerfProfiler->Pop();
    }

    Chunk& WorldGenerator::CreateChunk(const ChunkPos& pos)
    {
        // TODO: this and below flood the console
        // TODO: make the profiler just add the duration onto calls of the same name + increment a counter so we know how many times a method was called
        //Instance->PerfProfiler->Push("WorldGenerator::CreateChunk");

        // Avoid copying the chunk into the map, just create it in there and retrieve its reference
        m_World->Chunks.emplace(pos, Chunk(pos));
        auto& chunk = m_World->Chunks[pos];
        Generate(chunk);

        //Instance->PerfProfiler->Pop();

        return chunk;
    }

    // This is to stop a single block always getting a good random value
    // This would mean chests that spawn tend to be filled with good loot
    void WorldGenerator::Generate(Chunk& chunk)
    {
        //Instance->PerfProfiler->Push("WorldGenerator::GenerateChunk");

        GenerateBiomes(chunk);
        GenerateTerrain(chunk);
        GenerateCaves(chunk);
        GenerateStructures(chunk);
        GenerateDecorations(chunk);

        //Instance->PerfProfiler->Pop();
    }
}
