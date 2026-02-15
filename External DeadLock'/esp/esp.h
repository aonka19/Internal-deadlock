#pragma once
#include "../includes.h"

bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    const float* vm,
    int screenW,
    int screenH
);

void DrawESP(const float* viewMatrix);
