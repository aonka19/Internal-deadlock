#pragma once
#include "../includes.h"

bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    const float* vm,
    int screenW,
    int screenH
);

namespace ESP
{
    enum class BoxStyle
    {
        Full,
        Corners
    };

    struct Settings
    {
        ImVec4 enemyColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 teamColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        float  thickness = 1.5f;
        BoxStyle boxStyle = BoxStyle::Full;
        float   cornerFrac = 0.25f; // длина уголка от стороны
    };

    extern Settings g_Settings;

    void DrawESP(const float* viewMatrix);
    void DrawMenu();
}
