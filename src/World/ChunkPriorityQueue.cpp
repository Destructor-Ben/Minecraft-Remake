#include "ChunkPriorityQueue.h"

namespace Minecraft
{
    // TODO: what if we push with low priority, then push the same chunk with a higher priority? we would want to update the priority
    void ChunkPriorityQueue::push(Chunk* chunk, int priority)
    {
        if (m_ChunkSet.contains(chunk))
            return;

        m_ChunkSet.insert(chunk);
        m_PriorityQueue.push({ chunk, priority });
    }

    Chunk* ChunkPriorityQueue::pop()
    {
        if (m_PriorityQueue.empty())
            return nullptr;

        auto entry = m_PriorityQueue.top();
        m_PriorityQueue.pop();
        m_ChunkSet.erase(entry.ChunkPtr);
        return entry.ChunkPtr;
    }
}
