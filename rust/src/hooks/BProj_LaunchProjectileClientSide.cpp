#include "hooks.hpp"
#include <iostream>
void hk__BProj_LaunchProjectileClientside(BaseProjectile* instance, ItemDefinition* ammo, int projectileCount, float projSpreadaimCone)
{
	std::cout << "LaunchProjectileClientside!\n";
	ORIG(hk__BProj_LaunchProjectileClientside);

	auto pos = instance->transform()->position();
	std::cout << "current : " << pos << "\n";

	pos += UnityEngine::Vector3(0, 50, 0);
	std::cout << "new : " << pos << "\n";

	instance->transform()->position(pos);
	orig(instance, ammo, projectileCount, projSpreadaimCone);
}
