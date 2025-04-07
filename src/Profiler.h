#pragma once

namespace Minecraft
{
    struct ProfilerData
    {
        string Name;
        int Ms;
        ProfilerData* Parent;
        vector <ProfilerData> Children;

        string ToString(int level = 0) const;
    };

    class Profiler
    {
    public:
        void BeginFrame(string name);
        ProfilerData EndFrame();

        void Push(string name);
        void Pop();

    private:
        struct Scope
        {
            string Name;
            chrono::high_resolution_clock::time_point StartTime;
        };

        stack<Scope> m_Scopes = { };
        ProfilerData* m_CurrentData = nullptr;
        optional <ProfilerData> m_Data = nullopt;
    };
}
