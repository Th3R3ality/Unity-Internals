#include "hooks.hpp"

void hk__BP_OnLand(BasePlayer* instance, float fVelocity)
{
	ORIG(hk__BP_OnLand);

	if (fVelocity == -999) {
		orig(instance, fVelocity);
		return orig(instance, fVelocity);
	}
	return orig(instance, 0);
}
