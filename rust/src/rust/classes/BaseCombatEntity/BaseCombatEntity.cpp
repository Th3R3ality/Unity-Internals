#include "BaseCombatEntity.hpp"

#include "Il2cppLib.h"

BaseCombatEntity::LifeState BaseCombatEntity::lifestate()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BaseCombatEntity.lifestate");

	if (offset)
		return *(LifeState*)((uintptr_t)this + offset);
}

void BaseCombatEntity::lifestate(BaseCombatEntity::LifeState state)
{

}