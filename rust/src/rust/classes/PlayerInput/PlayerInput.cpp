#include "PlayerInput.hpp"
#include "Il2cppLib.h"

void PlayerInput::bodyAngles(UnityEngine::Vector3 angles)
{
	static auto offset = Il2cppLib::field_offset_from_signature("PlayerInput.bodyAngles");
	if (offset)
		*(UnityEngine::Vector3*)((uintptr_t)this + offset) = angles;
}

UnityEngine::Vector3 PlayerInput::bodyAngles()
{
	static auto offset = Il2cppLib::field_offset_from_signature("PlayerInput.bodyAngles");
	if (offset)
		return *(UnityEngine::Vector3*)((uintptr_t)this + offset);
}
