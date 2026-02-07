#pragma once
#include "../includes.h"

class SceneNode
{
public:
	char pad_0000[128]; //0x0000
	Vector3 absVecPos; //0x0080
	char pad_008C[256]; //0x008C
};

class PlayerPawn
{
public:
	char pad_0000[816]; //0x0000
	class SceneNode* pSceneNode; //0x0330
	char pad_0338[24]; //0x0338
	int32_t maxHealth; //0x0350
	int32_t health; //0x0354
	char pad_0358[4]; //0x0358
	int8_t lifeState; //0x035C
	char pad_035D[256]; //0x035D
};