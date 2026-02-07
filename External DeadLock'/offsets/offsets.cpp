#include "offsets.h"


namespace offset
{

    uintptr_t clientDLL = (uintptr_t)GetModuleHandleA("client.dll");

    uintptr_t CPlayerPawn = *(uintptr_t*)(clientDLL + 0x31E9488);

    uintptr_t CGameEntitySystem = *(uintptr_t*)(clientDLL + 0x3172C38);
    uintptr_t EntityList2 = CGameEntitySystem + 0x10;

    //ViewMatrix* viewMatrix = reinterpret_cast<ViewMatrix*>(clientDLL + 0x3728060);
    //inline uintptr_t viewMatrix = clientDLL + 0x3728060; // адрес начала float[16]
    float* viewMatrix = reinterpret_cast<float*>(clientDLL + 0x37280E0);
}

/*
namespace ViewMatrix
{
    float* viewMatrix = reinterpret_cast<float*>(offset::clientDLL + 0x3728060);
}
*/