#include "hooks.hpp"

void hk__HE_AddPunch(HeldEntity* instance, UnityEngine::Vector3 amount, float duration)
{
	ORIG(hk__HE_AddPunch);
	orig(instance, amount * 0, duration);
}