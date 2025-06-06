#include "Profiler.h"

#include "Game.h"
#include "Logger.h"
#include "Input/Input.h"

// Set to false so all calls to the profiler contain nothing and are hopefully inlined
#define PROFILER_ENABLED true

namespace Minecraft
{
    // This should always run because it is way less likely to affect performance so we can measure if the profiler affects it at all
    void Profiler::RecordFrameOrTickRate(bool isFrameRate)
    {
        // Record the frame/tick rate
        auto& data = isFrameRate ? m_FrameRateData : m_TickRateData;
        float time = isFrameRate ? Instance->DeltaTime : Instance->TickDeltaTime;
        float rate = 1.0f / time;
        data.push_back(rate);

        // Ensure the old data is deleted
        while (data.size() > 60)
        {
            data.erase(data.cbegin());
        }

        // Calculate the frame rates
        float& currentRate = isFrameRate ? m_CurrentFrameRate : m_CurrentTickRate;
        float& avgRate = isFrameRate ? m_AvgFrameRate : m_AvgTickRate;
        float& minRate = isFrameRate ? m_MinFrameRate : m_MinTickRate;

        currentRate = rate;
        avgRate = 0;
        minRate = std::numeric_limits<float>::infinity();

        for (float oldRate : data)
        {
            minRate = std::min(minRate, oldRate);
            avgRate += oldRate;
        }

        avgRate /= (float)data.size();
    }

    #if PROFILER_ENABLED

    void Profiler::BeginFrame(string name)
    {
        m_Data.clear();
        m_Scopes.clear();
        m_CurrentLevel = 0;
        Push(name);
    }

    vector <ProfilerData> Profiler::EndFrame()
    {
        Pop();

        if (m_CurrentLevel != 0)
            Logger::Throw("More pushes than pops were made on the profiler");

        return m_Data;
    }

    void Profiler::Push(string name)
    {
        // Check if the function has already been called
        if (m_Data.size() > 0)
        {
            auto& existingData = m_Data.back();
            if (existingData.Name == name)
            {
                // Shouldn't create new data
                m_Scopes.push_back(m_Data.size() - 1);
                existingData.StartTime = chrono::high_resolution_clock::now();
                m_CurrentLevel++; // Since it always gets subtracted even if it wasn't necessarily incremented
                return;
            }
        }

        // Create the data since the function hasn't been called yet
        auto& data = m_Data.emplace_back(name);
        data.Level = m_CurrentLevel;
        m_CurrentLevel++;
        m_Scopes.push_back(m_Data.size() - 1);
        data.StartTime = chrono::high_resolution_clock::now(); // Do this last
    }

    void Profiler::Pop()
    {
        // Measure time first
        auto endTime = chrono::high_resolution_clock::now();

        if (m_Data.size() <= 0)
            Logger::Throw("Called pop too many times on the profiler");

        auto& currentData = m_Data[m_Scopes.back()];
        currentData.Ms += std::chrono::duration<float, std::milli>(endTime - currentData.StartTime).count();
        currentData.Calls++;
        m_CurrentLevel--;
        m_Scopes.pop_back();
    }

    // TODO: processing the data - just keep recording it, since it is always being recorded anyway
    // TODO: if the frame time is high, just print the data to the console
    void Profiler::HandleProfilerData(const vector <ProfilerData>& data, ProfilerTarget target)
    {

    }

    string Profiler::ToString(const vector <ProfilerData>& data)
    {
        string output = "";

        for (auto& function : data)
        {
            for (int i = 0; i < function.Level * 2; ++i)
            {
                output += " ";
            }

            output += format("[{}]", function.Name);
            if (function.Calls > 1)
                output += format(" (x{})", function.Calls);

            output += format(" {:.3f}ms", function.Ms);
            output += "\n";
        }

        return output;
    }

    #else

    void Profiler::BeginFrame(string name) { }

    vector <ProfilerData> Profiler::EndFrame() { return { }; }

    void Profiler::Push(string name) { }

    void Profiler::Pop() { }

    void Profiler::HandleProfilerData(const vector <ProfilerData>& data, ProfilerTarget target) { }

    string Profiler::ToString(const vector <ProfilerData>& data) { return ""; }

    #endif
}
