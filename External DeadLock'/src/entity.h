#pragma once
#include "../includes.h"
#include "Player.h"

class EntitySystem
{
public:
	uintptr_t GetController(int id);
	PlayerPawn* GetPawn(int id);
};