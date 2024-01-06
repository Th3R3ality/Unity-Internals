#include "hooks.hpp"
#include "cache.hpp"
#include "sex.h"
#include <iostream>

bool hk__Proj_DoHit(Projectile* instance, HitTest* test, UnityEngine::Vector3 point, UnityEngine::Vector3 normal)
{
	ORIG(hk__Proj_DoHit);
	std::cout << "DoHit\n";

	auto it = std::find(manippedProjectiles.begin(), manippedProjectiles.end(), instance);

	if (it != manippedProjectiles.end()) {
		manippedProjectiles.erase(it);
		std::cout << "removed manippedProjectile : " << instance << "\n";
	}

	cache::debugDraw("LastHit", cache::debugDrawable(Lapis::Transform(point, 0, 0.1f), "#ff9050", Lapis::Shape::Icosahedron));
	return orig(instance, test, point, normal);
}