#pragma once
#include "../includes.h"

class SceneNode
{
public:
    char pad_0000[128]; //0x0000
    Vector3 absVecPos;  //0x0080
    char pad_008C[256]; //0x008C
};

class PlayerPawn
{
public:
    char pad_0000[816];        //0x0000
    SceneNode* pSceneNode;     //0x0330
    char pad_0338[24];         //0x0338
    int32_t maxHealth;         //0x0350
    int32_t health;            //0x0354
    char pad_0358[4];          //0x0358
    int8_t lifeState;          //0x035C
    char pad_035D[0x96];
    uint8_t teamNum;           //0x03F3
    char pad_03F4[0xC];
    uint8_t fFlags;            //0x0400

public:
    bool IsAlive() const
    {
        return health > 0 && lifeState == 0;
    }

    bool IsCrouching() const
    {
        // подставь правильный флаг под свою игру
        constexpr uint8_t FL_DUCKING = 1 << 1;
        return (fFlags & FL_DUCKING) != 0;
    }

    SceneNode* GetNode() const
    {
        return pSceneNode;
    }

    Vector3 GetFeet() const
    {
        return pSceneNode ? pSceneNode->absVecPos : Vector3{};
    }

    Vector3 GetHead() const
    {
        Vector3 head = GetFeet();
        // стоя / присед
        const float standOffset = 90.0f;
        const float crouchOffset = 60.0f; // подгони под игру
        head.z += IsCrouching() ? crouchOffset : standOffset;
        return head;
    }

    uint8_t GetTeam() const
    {
        return teamNum;
    }
};