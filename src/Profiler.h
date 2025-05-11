#pragma once

// TODO just use a flat array instead of a tree structure, it is much easier to deal with
namespace Minecraft
{
    struct ProfilerData
    {
        string Name;
        float Ms;
        ProfilerData* Parent;
        vector<ProfilerData> Children;

        string ToString(int level = 0) const;
    };

    // TODO: add stuff to save + average the profile data every frame, since it's always recording it anyway
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
        optional<ProfilerData> m_Data = nullopt;
    };
}
