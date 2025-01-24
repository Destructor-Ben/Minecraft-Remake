#include "WorldGenerator.h"

#include "Game.h"
#include "LogManager.h"
#include "World/World.h"
#include "World/Blocks/Blocks.h"

namespace Minecraft
{
    WorldGenerator::WorldGenerator(World* world, uint seed) :
        m_World(world),
        m_Seed(seed),
        m_Noise(seed) { }

    void WorldGenerator::Generate(int spawnRadius, int minHeight, int maxHeight)
    {
        for (int x = -spawnRadius + 1; x < spawnRadius; ++x)
        {
            for (int z = -spawnRadius + 1; z < spawnRadius; ++z)
            {
                for (int y = minHeight; y <= maxHeight; ++y)
                {
                    auto chunkPos = vec3i(x, y, z);
                    Chunk chunk(x, y, z);
                    Generate(chunk);
                    m_World->Chunks[chunkPos] = chunk;
                }
            }
        }
    }

    void WorldGenerator::GenerateChunksAroundPlayer(vec3 playerPos, int radius, int minHeight, int maxHeight)
    {
        auto playerChunkPos = WorldToChunkPos(playerPos);

        // When generating chunks side by side, many chunks will probably get re-meshed multiple times
        // So we queue them and use a set
        auto regenerateMeshQueue = set<Chunk*>();

        for (int x = -radius + 1; x < radius; ++x)
        {
            for (int y = -radius + 1; y < radius; ++y)
            {
                for (int z = -radius + 1; z < radius; ++z)
                {
                    // Calculate chunk pos
                    auto chunkPos = vec3i(x, y, z);
                    chunkPos += playerChunkPos;

                    // Check if the y position is out of bounds
                    if (chunkPos.y < minHeight || chunkPos.y > maxHeight)
                        continue;

                    // Check if the chunk exists
                    auto existingChunk = m_World->GetChunk(chunkPos);
                    if (existingChunk.has_value())
                        continue;

                    // Generate the chunk
                    Chunk chunk(chunkPos.x, chunkPos.y, chunkPos.z);
                    Generate(chunk); // Fucking value references get me sometimes, this needs to go BEFORE we set it, otherwise the empty chunk is copied to the Chunks map
                    m_World->Chunks[chunkPos] = chunk;

                    // Add chunks to re-mesh queue
                    AddChunkIfExists(regenerateMeshQueue, chunkPos);

                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(1, 0, 0));
                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(-1, 0, 0));

                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 1, 0));
                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, -1, 0));

                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 0, 1));
                    AddChunkIfExists(regenerateMeshQueue, chunkPos + vec3i(0, 0, -1));
                }
            }
        }

        // Regenerate chunk meshes
        for (auto chunk : regenerateMeshQueue)
        {
            chunk->RegenerateMesh();
        }
    }

    void WorldGenerator::AddChunkIfExists(set<Chunk*>& chunks, vec3i chunkPos)
    {
        auto chunk = m_World->GetChunk(chunkPos);
        if (chunk.has_value())
            chunks.insert(chunk.value());
    }

    // TODO: make multiple cpp files for this to simplify this file
    // TODO: to avoid issues with chunk borders, possibly just make the game just ask each block what type it should be
    // And do this repeatedly
    // To handle structures, just generate a bunch of points beforehand
    // TODO: make random number/bool generation easier with a util method that offsets the input coordinates
    // This is to stop a single block always getting a good random value
    // This would mean chests that spawn tend to be filled with good loot
    // TODO: make a NoiseMap object that will store the parameters and can be sampled easily?
    void WorldGenerator::Generate(Chunk& chunk)
    {
        // Generate terrain
        const int NoiseScale = 25.0f;
        const int Height = 25.0f;

        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                // Generate height
                float xCoord = x + chunk.GetWorldPos().x;
                float zCoord = z + chunk.GetWorldPos().z;

                xCoord /= NoiseScale;
                zCoord /= NoiseScale;

                float noiseValue = m_Noise.Fractal2D(xCoord, zCoord);
                int height = (int)(noiseValue * Height);

                // Set blocks
                for (int y = 0; y < Chunk::Size; ++y)
                {
                    Block block = chunk.GetBlock(x, y, z);
                    float yPos = block.GetWorldPos().y;

                    // Set the block type
                    auto type = Blocks::Air.get();

                    if (yPos == height)
                        type = Blocks::Grass.get();

                    if (yPos < height)
                        type = Blocks::Dirt.get();

                    if (yPos <= height - 2)
                        type = Blocks::Stone.get();

                    block.Data.Type = type;
                }
            }
        }

        // Generate trees and grass
        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                for (int y = 0; y < Chunk::Size; y++)
                {
                    // Find a dirt or grass block with air above it
                    Block block = chunk.GetBlock(x, y, z);

                    auto blockType = block.Data.Type;
                    if (blockType != Blocks::Dirt.get() && blockType != Blocks::Grass.get())
                        continue;

                    int yAbove = y + 1;
                    // TODO: fix across chunk borders
                    if (yAbove >= Chunk::Size)
                        continue;

                    Block blockAbove = chunk.GetBlock(x, yAbove, z);
                    if (blockAbove.Data.Type != Blocks::Air.get())
                        continue;

                    // Place the thing
                    // TODO: proper random chances
                    const float Chance = 0.1f;
                    if (m_Noise.White3D(x, yAbove, z) > Chance)
                        continue;

                    blockAbove.Data.Type = Blocks::TallGrass.get();
                }
            }
        }

        // TODO: avoid copy paste
        for (int x = 0; x < Chunk::Size; x++)
        {
            for (int z = 0; z < Chunk::Size; z++)
            {
                for (int y = 0; y < Chunk::Size; y++)
                {
                    // Find a dirt or grass block with air above it
                    Block block = chunk.GetBlock(x, y, z);

                    auto blockType = block.Data.Type;
                    if (blockType != Blocks::Dirt.get() && blockType != Blocks::Grass.get())
                        continue;

                    int yAbove = y + 1;
                    // TODO: fix across chunk borders
                    if (yAbove >= Chunk::Size)
                        continue;

                    Block blockAbove = chunk.GetBlock(x, yAbove, z);
                    if (blockAbove.Data.Type != Blocks::Air.get())
                        continue;

                    // Place the thing
                    // TODO: proper random chances - Since this uses the same noise map as the grass, it will fail to place when the chances are the same
                    const float Chance = 0.01f;
                    if (m_Noise.White3D(x + 1024, yAbove + 1024, z + 1024) > Chance)
                        continue;

                    int height = round(glm::lerp(5.0f, 7.0f, m_Noise.White3D(x + 2048, y + 2048, z + 2048)));

                    // Trunk
                    for (int yTrunk = 1; yTrunk <= height; ++yTrunk)
                    {
                        // TODO: fix across chunk borders
                        if (y + yTrunk >= Chunk::Size)
                            continue;

                        Block trunkBlock = chunk.GetBlock(x, y + yTrunk, z);
                        trunkBlock.Data.Type = Blocks::Wood.get();
                    }

                    // Leaves
                    for (int leafX = -1; leafX <= 1; ++leafX)
                    {
                        for (int leafZ = -1; leafZ <= 1; ++leafZ)
                        {
                            for (int leafY = -1; leafY <= 1; ++leafY)
                            {
                                int leafFinalX = x + leafX;
                                int leafFinalY = y + height + leafY;
                                int leafFinalZ = z + leafZ;

                                // TODO: fix across chunk borders
                                if (leafFinalX >= Chunk::Size || leafFinalX < 0)
                                    continue;

                                if (leafFinalY >= Chunk::Size || leafFinalY < 0)
                                    continue;

                                if (leafFinalZ >= Chunk::Size || leafFinalZ < 0)
                                    continue;

                                Block leafBlock = chunk.GetBlock(leafFinalX, leafFinalY, leafFinalZ);

                                if (leafBlock.Data.Type == Blocks::Air.get())
                                    leafBlock.Data.Type = Blocks::Leaves.get();
                            }
                        }
                    }

                    for (int leafX = -2; leafX <= 2; ++leafX)
                    {
                        for (int leafZ = -2; leafZ <= 2; ++leafZ)
                        {
                            for (int leafY = -2; leafY <= -1; ++leafY)
                            {
                                int leafFinalX = x + leafX;
                                int leafFinalY = y + height + leafY;
                                int leafFinalZ = z + leafZ;

                                // TODO: fix across chunk borders
                                if (leafFinalX >= Chunk::Size || leafFinalX < 0)
                                    continue;

                                if (leafFinalY >= Chunk::Size || leafFinalY < 0)
                                    continue;

                                if (leafFinalZ >= Chunk::Size || leafFinalZ < 0)
                                    continue;

                                Block leafBlock = chunk.GetBlock(leafFinalX, leafFinalY, leafFinalZ);

                                if (leafBlock.Data.Type == Blocks::Air.get())
                                    leafBlock.Data.Type = Blocks::Leaves.get();
                            }
                        }
                    }
                }
            }
        }
    }
}
