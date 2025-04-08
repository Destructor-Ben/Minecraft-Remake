#include "Profiler.h"

#include "Game.h"
#include "LogManager.h"

namespace Minecraft
{
    string ProfilerData::ToString(int level) const
    {
        string heading = format("{} {:.3f}ms", Name, Ms);
        for (int i = 0; i < level; ++i)
        {
            heading = "  " + heading;
        }

        string children = "";
        for (auto child : Children)
        {
            children += child.ToString(level + 1);
        }

        return heading + "\n" + children;
    }

    void Profiler::BeginFrame(string name)
    {
        if (m_Data.has_value())
            Instance->Logger->Throw("Started a profiler frame while another is already running");

        m_Data = ProfilerData("<root>");
        m_Scopes = { };
        m_CurrentData = &m_Data.value();

        Push(name);
    }

    ProfilerData Profiler::EndFrame()
    {
        if (!m_Data.has_value())
            Instance->Logger->Throw("Tried to end a profiler frame while there isn't one");

        Pop();

        auto data = m_Data.value();
        m_Data = nullopt;

        if (data.Children.size() != 1)
            Instance->Logger->Throw("Profiler went badly wrong");

        return data.Children.at(0);
    }

    void Profiler::Push(string name)
    {
        if (!m_Data.has_value())
            Instance->Logger->Throw("Profiler frame hasn't started yet");

        // Create the data
        auto parent = m_CurrentData;
        m_CurrentData = &m_CurrentData->Children.emplace_back(name);
        m_CurrentData->Parent = parent;

        // Do this last
        auto startTime = chrono::high_resolution_clock::now();
        m_Scopes.emplace(name, startTime);
    }

    void Profiler::Pop()
    {
        if (!m_Data.has_value())
            Instance->Logger->Throw("Profiler frame hasn't started yet");

        // Measure time first
        auto endTime = chrono::high_resolution_clock::now();
        auto scope = m_Scopes.top();

        // Add to profile results
        m_CurrentData->Ms = std::chrono::duration<float, std::milli>(endTime - scope.StartTime).count();

        // Update current data
        m_CurrentData = m_CurrentData->Parent;

        // Pop
        m_Scopes.pop();
    }
}
