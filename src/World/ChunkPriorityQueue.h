#pragma once

namespace Minecraft
{
    class Chunk;

    // FIFO, priority, and no duplicates queue for chunks
    class ChunkPriorityQueue
    {
    public:
        void push(Chunk* chunk, int priority);
        Chunk* pop();

        bool empty() const { return m_PriorityQueue.empty(); }

    private:
        struct ChunkEntry
        {
            Chunk* ChunkPtr;
            int Priority;

            // Define comparison for the priority queue (higher priority first)
            bool operator <(const ChunkEntry& other) const
            {
                return Priority < other.Priority;
            }
        };

        priority_queue<ChunkEntry> m_PriorityQueue;
        unordered_set<Chunk*> m_ChunkSet;
    };
}
