#pragma once

namespace Minecraft
{
    class LogManager;
    class InputManager;
    class Renderer;
    class World;

    inline shared_ptr<LogManager> Logger = nullptr;
    inline shared_ptr<InputManager> Input = nullptr;
    inline shared_ptr<Renderer> Renderer = nullptr;
    inline shared_ptr<World> World = nullptr;

    inline bool Running = true;
    inline std::thread::id MainThreadID = std::thread::id();
    inline shared_ptr<std::thread> TickThread = nullptr;

    string ReadResourceText(string path);
    vector<uint8> ReadResourceBytes(string path);
}
