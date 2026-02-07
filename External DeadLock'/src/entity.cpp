#include <entity.h>

	// Get LocalPlayerController by index (ID)
uintptr_t EntitySystem::GetController(int id)
{
	uintptr_t chunk = *(uintptr_t*)(offset::CGameEntitySystem + 0x10 + 8 * (id >> 9));	// looking for the right chunk

	return *(uintptr_t*)(chunk + 112ull * (id & 0x1FF));
}

	// Get LocalPlayerPawn by
PlayerPawn* EntitySystem::GetPawn(int id)
{
	uintptr_t controller = GetController(id);
	if (!controller) return 0;

	int hPawn = *(int*)(controller + 0x6BC);
	if (hPawn < 0) return 0;

	uintptr_t chunk = *(uintptr_t*)(offset::EntityList2 + 8ull * ((hPawn & 0x7FFF) >> 9));
	if (!chunk) return 0;

	uintptr_t playerPawn = *(uintptr_t*)(chunk + 112ull * (hPawn & 0x1FF));
	if (!playerPawn) return 0;

	return reinterpret_cast<PlayerPawn*>(playerPawn);
}