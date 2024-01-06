#include "hooks.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include "cache.hpp"
#include "sex.h"

std::vector<Projectile*> manippedProjectiles;

void hk__Proj_UpdateVelocity(Projectile* instance, float deltaTime)
{
	//std::cout << "Proj_UpdateVelocity\n";

	ORIG(hk__Proj_UpdateVelocity);

	auto lp = cache::local();
	auto cam = cache::cameraMain();

	//std::wcout << "owner : " << instance->owner()->name() << "\n";

	if (instance->owner() == lp && cam)
	{
		//std::cout << "has sex? : " << sexbool << "\n";
		if (sexbool) { // || std::find(manippedProjectiles.begin(), manippedProjectiles.end(), instance) == manippedProjectiles.end()) {
			std::cout << "sexing bullar\n";
			auto pos = cam->transform()->position();

			instance->transform()->position(pos + UnityEngine::Vector3(0, 1, 0));

			//manippedProjectiles.push_back(instance);
		}
	}
	orig(instance, deltaTime);
}
