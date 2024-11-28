#pragma once

namespace Minecraft
{
    // TODO: cache resources, or maybe load them all
    class ResourceManager
    {
    public:
        string ReadResourceText(string path);
        vector<byte> ReadResourceBytes(string path);
    };
}
