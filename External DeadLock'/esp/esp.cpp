#include "../includes.h"
#include "Player.h"
#include "entity.h"

inline bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    const float* vm,
    int screenW,
    int screenH
)
{
    // row-major, как у тебя
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

void DrawESP(const float* viewMatrix)
{
    ImGuiIO& io = ImGui::GetIO();
    const int screenW = static_cast<int>(io.DisplaySize.x);
    const int screenH = static_cast<int>(io.DisplaySize.y);

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    // ВАЖНО: один EntitySystem, а не в цикле
    static EntitySystem ent;

    constexpr int kMaxPlayers = 64;
    constexpr float kMinBoxHeight = 5.0f;
    constexpr float kHeadOffset = 90.0f;
    constexpr float kBoxWidthRatio = 0.45f;

    for (int i = 2; i < kMaxPlayers; ++i)
    {
        PlayerPawn* pawn = ent.GetPawn(i);
        if (!pawn)
            continue;

        if (pawn->health <= 0 || pawn->lifeState != 0)
            continue;

        SceneNode* node = pawn->pSceneNode;
        if (!node)
            continue;

        const Vector3& feet = node->absVecPos;
        Vector3 head = feet;
        head.z += kHeadOffset;

        Vector2 feet2D, head2D;

        if (!WorldToScreen(feet, feet2D, viewMatrix, screenW, screenH))
            continue;

        if (!WorldToScreen(head, head2D, viewMatrix, screenW, screenH))
            continue;

        const float boxHeight = feet2D.y - head2D.y;
        if (boxHeight < kMinBoxHeight)
            continue;

        const float boxWidth = boxHeight * kBoxWidthRatio;

        const float x = head2D.x - boxWidth * 0.5f;
        const float y = head2D.y;

        draw->AddRect(
            ImVec2(x, y),
            ImVec2(x + boxWidth, y + boxHeight),
            IM_COL32(255, 0, 0, 255),
            0.0f,
            0,
            1.5f
        );
    }
}
