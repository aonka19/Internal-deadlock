#pragma once
#include "../includes.h"

bool WorldToScreen(
    const Vec3& world,
    Vector2& screen,
    float* vm,
    int screenW,
    int screenH
);

void DrawESP(float* viewMatrix);