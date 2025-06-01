#pragma once

// TODO just use a flat array instead of a tree structure, it is much easier to deal with
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
        float Ms;
        ProfilerData* Parent;
        vector <ProfilerData> Children;

        string ToString(int level = 0) const;
    };

    // TODO: add stuff to save + average the profile data every frame, since it's always recording it anyway
    // This is a class because we need a separate one for each thread
    class Profiler
    {
    public:
        vector<ProfilerData> TickPerfData = { };
        vector<ProfilerData> UpdatePerfData = { };
        vector<ProfilerData> RenderPerfData = { };

        void BeginFrame(string name);
        ProfilerData EndFrame();

        void Push(string name);
        void Pop();

        void HandleProfilerData(const ProfilerData& data, vector<ProfilerData>& previousData, ProfilerTarget target);

    private:
        struct Scope
        {
            string Name;
            chrono::high_resolution_clock::time_point StartTime;
        };

        stack<Scope> m_Scopes = { };
        ProfilerData* m_CurrentData = nullptr;
        optional<ProfilerData> m_Data = nullopt;
    };
}
