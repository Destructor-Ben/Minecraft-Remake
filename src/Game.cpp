#include "Game.h"

#include "Common.h"

namespace Minecraft
{
    InputManager* Input = nullptr;
    class Renderer* Renderer = nullptr;
    class World* World = nullptr;

    void Initialize()
    {
        Input = new InputManager();
        Renderer = new class Renderer();
        World = new class World();

        Renderer::UnbindAll();
    }

    void Shutdown()
    {
        Renderer::UnbindAll();

        delete World;
        delete Renderer;
        delete Input;
    }

    void Tick()
    {
        World->Tick();
    }

    void Update()
    {
        Input->Update();
        World->Update();
        Renderer->Update();

        Input->PostUpdate();
    }

    void Render()
    {
        Renderer::Clear();

        World->Render();
    }
}
