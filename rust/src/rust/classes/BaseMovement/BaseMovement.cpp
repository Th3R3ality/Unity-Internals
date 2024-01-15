#include "BaseMovement.hpp"
#include "Il2cppLib.h"

void BaseMovement::TargetMovement(UnityEngine::Vector3 v3)
{
	static auto fn = (void(*)(BaseMovement*, UnityEngine::Vector3))Il2cppLib::method_from_signature("BaseMovement.set_TargetMovement(Vector3)");
	if (fn)
		fn(this, v3);
}
