#include "ResourceManager.h"

#include "Game.h"
#include "LogManager.h"

namespace Minecraft
{
    string ResourceManager::ReadResourceText(string path)
    {
        std::ifstream stream(path);
        if (stream.fail())
            Instance->Logger->Throw("Failed to load resource at path: " + path);

        std::stringstream buffer;
        buffer << stream.rdbuf();
        stream.close();
        return buffer.str();
    }

    vector <byte> ResourceManager::ReadResourceBytes(string path)
    {
        std::ifstream stream(path, std::ios::binary);
        if (stream.fail())
            Instance->Logger->Throw("Failed to load resource at path: " + path);

        vector <byte> bytes((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        stream.close();
        return bytes;
    }
}
