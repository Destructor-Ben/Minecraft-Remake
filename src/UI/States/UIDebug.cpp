#include "UIDebug.h"

#include "Game.h"
#include "Version.h"
#include "Input/Input.h"
#include "Graphics/Renderers/ChunkRenderer.h"
#include "Graphics/Renderers/Renderer.h"
#include "Graphics/Renderers/TextRenderer.h"
#include "UI/Elements/UIText.h"
#include "World/World.h"

namespace Minecraft
{
    void UIDebug::OnInit()
    {
        Active = false;

        // === Info === /
        MakeText(format("Minecraft Remake v{}", Version::String), Colors::White);
        MakeText("Press [`] to toggle this menu", Colors::White);
        MakeText("[B] toggles chunk borders", Colors::White);
        MakeText("[G] toggles wireframes", Colors::White);

        // === Performance === //
        m_VSyncText = MakeText("VSync", Colors::RarityGreen);
        m_FrameRateText = MakeText("FPS", Colors::RarityGreen);
        m_TickRateText = MakeText("TPS", Colors::RarityGreen);

        // === Player === //
        m_PlayerPosText = MakeText("PlayerPos", Colors::RarityRed);
        m_PlayerChunkText = MakeText("PlayerChunk", Colors::RarityRed);
        m_PlayerBlockPosText = MakeText("PlayerBlockPos", Colors::RarityRed);
        m_PlayerDirectionText = MakeText("PlayerDirection", Colors::RarityRed);

        // === Targeted Block === //
        // TODO: finish these
        m_TargetedBlockPosText = MakeText("TargetBlockPos", Colors::RarityBlue);
        m_TargetedBlockTypeText = MakeText("TargetBlockType", Colors::RarityBlue);
    }

    shared_ptr <UIText> UIDebug::MakeText(string text, Color color)
    {
        auto element = make_shared<UIText>();
        element->SetText(text);
        element->Text.TextColor = color;
        element->x.Pixels = TextPadding;
        element->y.Pixels = -(TextPadding + m_CurrentTextYPos);
        element->y.Percent = 1;
        element->OriginY.Percent = 1;
        AddElement(element);

        m_CurrentTextYPos += TextPadding + TextRenderer::GetTextSize(text).y;

        return element;
    }

    void UIDebug::CheckActive()
    {
        // Toggling with the backtick key
        if (Input::WasKeyReleased(Key::Grave))
            Active = !Active;

        // Don't need an in game check, this should run in the main menu too
    }

    void UIDebug::OnUpdate()
    {
        // Enable and disable debug tools
        if (Instance->InGame && Input::WasKeyReleased(Key::B))
            Instance->ChunkGraphics->DrawChunkBorders = !Instance->ChunkGraphics->DrawChunkBorders;

        if (Input::WasKeyReleased(Key::G))
            Instance->Graphics->DrawWireframes = !Instance->Graphics->DrawWireframes;

        // Update the text
        // TODO: implement FPS and TPS graphs, implement profiler data graph
        m_VSyncText->SetText(format("VSync: {}", Instance->IsVSyncEnabled()));
        m_FrameRateText->SetText(format("FPS: Current={:.3f} Avg={:.3f} Min={:.3f} Target={:.1f}", Instance->PerfProfiler->GetCurrentFrameRate(), Instance->PerfProfiler->GetAvgFrameRate(), Instance->PerfProfiler->GetMinFrameRate(), Instance->TargetFrameRate));
        m_TickRateText->SetText(format("TPS: Current={:.3f} Avg={:.3f} Min={:.3f} Target={:.1f}", Instance->PerfProfiler->GetCurrentTickRate(), Instance->PerfProfiler->GetAvgTickRate(), Instance->PerfProfiler->GetMinTickRate(), Instance->TargetTickRate));

        if (!Instance->InGame)
        {
            m_PlayerPosText->SetText("");
            m_PlayerChunkText->SetText("");
            m_PlayerBlockPosText->SetText("");
            m_PlayerDirectionText->SetText("");
        }
        else
        {
            vec3 playerPos = Instance->CurrentWorld->PlayerCamera.Position;
            m_PlayerPosText->SetText(format("PlayerPos: ({},{},{}) ({:.3f},{:.3f},{:.3f})", (int)playerPos.x, (int)playerPos.y, (int)playerPos.z, playerPos.x, playerPos.y, playerPos.z));
            vec3i chunkPos = WorldToChunkPos(playerPos);
            m_PlayerChunkText->SetText(format("PlayerChunk: ({},{},{})", chunkPos.x, chunkPos.y, chunkPos.z));
            vec3i blockPos = WorldToBlockPos(playerPos);
            m_PlayerBlockPosText->SetText(format("BlockPos: ({},{},{})", blockPos.x, blockPos.y, blockPos.z));
            vec3 playerRot = glm::eulerAngles(Instance->CurrentWorld->PlayerCamera.Rotation);
            m_PlayerDirectionText->SetText(""); // TODO: idk how to do this
        }
    }
}
