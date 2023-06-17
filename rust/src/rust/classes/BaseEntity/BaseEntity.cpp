#include "BaseEntity.hpp"

#include "Il2cppLib.h"

Model* BaseEntity::model()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BaseEntity.model");
	return *(Model**)((uintptr_t)this + offset);
}