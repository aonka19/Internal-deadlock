#include "../includes.h"
#include "Player.h"
#include "entity.h"

bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    float* vm,
    int screenW,
    int screenH
)
{
    float clipX =
        world.x * vm[0] +
        world.y * vm[1] +
        world.z * vm[2] +
        vm[3];

    float clipY =
        world.x * vm[4] +
        world.y * vm[5] +
        world.z * vm[6] +
        vm[7];

    float clipW =
        world.x * vm[12] +
        world.y * vm[13] +
        world.z * vm[14] +
        vm[15];

    if (clipW <= 0.01f)
        return false;

    float invW = 1.0f / clipW;
    clipX *= invW;
    clipY *= invW;

    screen.x = (screenW * 0.5f) * (clipX + 1.0f);
    screen.y = (screenH * 0.5f) * (1.0f - clipY);

    return true;
}

void DrawESP(float* viewMatrix)
{
    ImGuiIO& io = ImGui::GetIO();
    int screenW = (int)io.DisplaySize.x;
    int screenH = (int)io.DisplaySize.y;

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    for (int i = 2; i < 64; i++) // max players
    {
        EntitySystem ent;
        PlayerPawn* pawn = ent.GetPawn(i);

        if (!pawn)
            continue;

        if (pawn->health <= 0 || pawn->lifeState != 0)
            continue;

        SceneNode* node = pawn->pSceneNode;
        if (!node)
            continue;

        Vector3 feet = node->absVecPos;
        Vector3 head = feet;
        head.z += 85.0f;

        Vector2 feet2D, head2D;

        if (!WorldToScreen(feet, feet2D, viewMatrix, screenW, screenH))
            continue;

        if (!WorldToScreen(head, head2D, viewMatrix, screenW, screenH))
            continue;

        float boxHeight = feet2D.y - head2D.y;
        if (boxHeight < 5.0f)
            continue;

        float boxWidth = boxHeight * 0.45f;

        float x = head2D.x - boxWidth * 0.5f;
        float y = head2D.y;

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
