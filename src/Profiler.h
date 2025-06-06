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
    // TODO: moving the mouse seems to lower the framerate
    // TODO: make sure this doesn't slow anything down
    // - Do an option in release mode where Push/Pop are blank and compare FPS
    class Profiler
    {
    public:
        void BeginFrame(string name);
        vector <ProfilerData> EndFrame();

        void Push(string name);
        void Pop();

        static void HandleProfilerData(const vector <ProfilerData>& data, ProfilerTarget target);
        static void RecordFrameOrTickRate(bool isFrameRate);
        static string ToString(const vector <ProfilerData>& data);

        // Getters for all of the frame debug info
        float GetCurrentFrameRate() const { return m_CurrentFrameRate; }
        float GetCurrentTickRate() const { return m_CurrentTickRate; }
        float GetAvgFrameRate() const { return m_AvgFrameRate; }
        float GetAvgTickRate() const { return m_AvgTickRate; }
        float GetMinFrameRate() const { return m_MinFrameRate; }
        float GetMinTickRate() const { return m_MinTickRate; }

    private:
        vector <ProfilerData> m_Data = { };
        vector<int> m_Scopes = { };
        int m_CurrentLevel = 0;

        // Data recorded while profiling
        inline static float m_CurrentFrameRate = 0;
        inline static float m_CurrentTickRate = 0;
        inline static float m_AvgFrameRate = 0;
        inline static float m_AvgTickRate = 0;
        inline static float m_MinFrameRate = 0;
        inline static float m_MinTickRate = 0;

        inline static vector<float> m_FrameRateData = { };
        inline static vector<float> m_TickRateData = { };
        inline static vector <vector<ProfilerData>> m_TickPerfData = { };
        inline static vector <vector<ProfilerData>> m_UpdatePerfData = { };
        inline static vector <vector<ProfilerData>> m_RenderPerfData = { };
    };
}
