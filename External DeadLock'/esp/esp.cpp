#include "../includes.h"
#include "Player.h"
#include "entity.h"
#include "esp.h"

ESP::Settings ESP::g_Settings;

inline bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    const float* vm,
    int screenW,
    int screenH
)
{
    const float clipX =
        world.x * vm[0] +
        world.y * vm[1] +
        world.z * vm[2] +
        vm[3];

    const float clipY =
        world.x * vm[4] +
        world.y * vm[5] +
        world.z * vm[6] +
        vm[7];

    const float clipW =
        world.x * vm[12] +
        world.y * vm[13] +
        world.z * vm[14] +
        vm[15];

    if (clipW <= 0.01f)
        return false;

    const float invW = 1.0f / clipW;
    const float ndcX = clipX * invW;
    const float ndcY = clipY * invW;

    const float halfW = screenW * 0.5f;
    const float halfH = screenH * 0.5f;

    screen.x = halfW * (ndcX + 1.0f);
    screen.y = halfH * (1.0f - ndcY);

    return true;
}

namespace
{
    ImU32 ToImU32(const ImVec4& c)
    {
        return ImGui::ColorConvertFloat4ToU32(c);
    }

    ImU32 GetColorForPlayer(const PlayerPawn* pawn, const PlayerPawn* local)
    {
        if (!local)
            return ToImU32(ESP::g_Settings.enemyColor);

        if (pawn->GetTeam() == local->GetTeam())
            return ToImU32(ESP::g_Settings.teamColor);

        return ToImU32(ESP::g_Settings.enemyColor);
    }

    void DrawBoxFull(ImDrawList* draw, float x, float y, float w, float h, ImU32 col, float thickness)
    {
        draw->AddRect(
            ImVec2(x, y),
            ImVec2(x + w, y + h),
            col,
            0.0f,
            0,
            thickness
        );
    }

    void DrawBoxCorners(ImDrawList* draw, float x, float y, float w, float h, ImU32 col, float thickness, float frac)
    {
        const float lw = w * frac;
        const float lh = h * frac;

        const ImVec2 p1(x, y);
        const ImVec2 p2(x + w, y);
        const ImVec2 p3(x, y + h);
        const ImVec2 p4(x + w, y + h);

        // top-left
        draw->AddLine(p1, ImVec2(p1.x + lw, p1.y), col, thickness);
        draw->AddLine(p1, ImVec2(p1.x, p1.y + lh), col, thickness);

        // top-right
        draw->AddLine(p2, ImVec2(p2.x - lw, p2.y), col, thickness);
        draw->AddLine(p2, ImVec2(p2.x, p2.y + lh), col, thickness);

        // bottom-left
        draw->AddLine(p3, ImVec2(p3.x + lw, p3.y), col, thickness);
        draw->AddLine(p3, ImVec2(p3.x, p3.y - lh), col, thickness);

        // bottom-right
        draw->AddLine(p4, ImVec2(p4.x - lw, p4.y), col, thickness);
        draw->AddLine(p4, ImVec2(p4.x, p4.y - lh), col, thickness);
    }
}

void ESP::DrawESP(const float* viewMatrix)
{
    ImGuiIO& io = ImGui::GetIO();
    const int screenW = static_cast<int>(io.DisplaySize.x);
    const int screenH = static_cast<int>(io.DisplaySize.y);

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    static EntitySystem ent;

    constexpr int   kMaxPlayers = 64;
    constexpr float kMinBoxHeight = 5.0f;
    constexpr float kBoxWidthRatio = 0.45f;

    // локальный игрок (для цвета)
    PlayerPawn* local = ent.GetPawn(1); // подставь свой индекс локала

    for (int i = 2; i < kMaxPlayers; ++i)
    {
        PlayerPawn* pawn = ent.GetPawn(i);
        if (!pawn)
            continue;

        if (!pawn->IsAlive())
            continue;

        if (!pawn->GetNode())
            continue;

        const Vector3 feet3D = pawn->GetFeet();
        const Vector3 head3D = pawn->GetHead();

        Vector2 feet2D, head2D;

        if (!WorldToScreen(feet3D, feet2D, viewMatrix, screenW, screenH))
            continue;

        if (!WorldToScreen(head3D, head2D, viewMatrix, screenW, screenH))
            continue;

        const float boxHeight = feet2D.y - head2D.y;
        if (boxHeight < kMinBoxHeight)
            continue;

        const float boxWidth = boxHeight * kBoxWidthRatio;

        const float x = head2D.x - boxWidth * 0.5f;
        const float y = head2D.y;

        ImU32 col = GetColorForPlayer(pawn, local);

        switch (g_Settings.boxStyle)
        {
        case BoxStyle::Full:
            DrawBoxFull(draw, x, y, boxWidth, boxHeight, col, g_Settings.thickness);
            break;
        case BoxStyle::Corners:
            DrawBoxCorners(draw, x, y, boxWidth, boxHeight, col, g_Settings.thickness, g_Settings.cornerFrac);
            break;
        }
    }
}

void ESP::DrawMenu()
{
    if (ImGui::Begin("NoMore"))
    {
        ImGui::Text("ESP settings");

        ImGui::ColorEdit4("Enemy color", (float*)&g_Settings.enemyColor);
        ImGui::ColorEdit4("Team color", (float*)&g_Settings.teamColor);

        ImGui::SliderFloat("Thickness", &g_Settings.thickness, 0.5f, 5.0f, "%.1f");
        ImGui::SliderFloat("Corner frac", &g_Settings.cornerFrac, 0.1f, 0.5f, "%.2f");

        const char* styles[] = { "Full box", "Corners" };
        int style = (g_Settings.boxStyle == BoxStyle::Full) ? 0 : 1;
        if (ImGui::Combo("Box style", &style, styles, IM_ARRAYSIZE(styles)))
        {
            g_Settings.boxStyle = (style == 0) ? BoxStyle::Full : BoxStyle::Corners;
        }

        ImGui::End();
    }
}