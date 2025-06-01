#pragma once

namespace Minecraft
{
    enum class ProfilerTarget
    {
        Tick,
        Update,
        Render,
    };

    struct ProfilerData
    {
        string Name;
        float Ms = 0.0f;
        int Calls = 0;
        int Level = 0;

        // Only used for calculations, not intended to be read from
        chrono::high_resolution_clock::time_point StartTime;
    };

    // This is a class because we need a separate one for each thread
    // TODO: make sure this doesn't slow anything down
    class Profiler
    {
    public:
        static vector <ProfilerData> TickPerfData;
        static vector <ProfilerData> UpdatePerfData;
        static vector <ProfilerData> RenderPerfData;

        void BeginFrame(string name);
        vector <ProfilerData> EndFrame();

        void Push(string name);
        void Pop();

        static void HandleProfilerData(const vector <ProfilerData>& data, vector <ProfilerData>& previousData, ProfilerTarget target);
        static string ToString(const vector <ProfilerData>& data);

    private:
        vector <ProfilerData> m_Data = { };
        vector<int> m_Scopes = { };
        int m_CurrentLevel = 0;
    };
}
