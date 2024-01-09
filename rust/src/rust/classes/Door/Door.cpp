#include "Door.hpp"
#include "Il2cppLib.h"

bool Door::hasHatch()
{
    static auto fn = Il2cppLib::field_offset_from_signature("Door.hasHatch");
    if (fn)
        return *(bool*)((uintptr_t)this + fn);
    return false;
}
