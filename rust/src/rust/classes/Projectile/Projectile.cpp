#include "Projectile.hpp"
#include "Il2cppLib.h"

BasePlayer* Projectile::owner()
{
    static auto offset = Il2cppLib::field_offset_from_signature("Projectile.owner");
    if (offset)
        return *(BasePlayer**)((uintptr_t)this + offset);
    return nullptr;
}