#include "hooks.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache.hpp"

std::vector<Projectile*> manippedProjectiles;

void hk__Proj_UpdateVelocity(Projectile* instance, float deltaTime)
{
	std::cout << "Proj_UpdateVelocity\n";

	ORIG(hk__Proj_UpdateVelocity);

	auto lp = cache::local();

	//std::wcout << "owner : " << instance->owner()->name() << "\n";

	if (instance->owner() == lp && std::find(manippedProjectiles.begin(), manippedProjectiles.end(), instance) == manippedProjectiles.end()) {

		auto pos = lp->transform()->position();

		pos += UnityEngine::Vector3(0, 5, 0);

		instance->transform()->position(pos);

		manippedProjectiles.push_back(instance);
	}

	orig(instance, deltaTime);
}
