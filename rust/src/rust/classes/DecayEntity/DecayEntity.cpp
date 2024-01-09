#include "DecayEntity.hpp"

unsigned int DecayEntity::buildingID()
{
    static auto offset = Il2cppLib::field_offset_from_signature("DecayEntity.buildingID");
    if (offset)
        return *(unsigned int*)((uintptr_t)this + offset);
    return 0;
}