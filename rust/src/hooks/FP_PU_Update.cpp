#include "hooks.hpp"

#include <iostream>
#include <Windows.h>

#include "cheat.hpp"
#include "config.hpp"

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{
	static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
		std::cout << orig << " : orig much???" << std::endl;
		return;
	}
	orig(instance);

	static bool actually_unloading = false;
	if (!actually_unloading && cheat::state() == cheat::status::unloading) {
		actually_unloading = true;
		//cheat::unload_gameObjects();
		cheat::revert_model_changes();
		cheat::unload_assetbundles();
		cheat::has_unloaded(true);
	}

	auto localPlayer = cache::local();

	if (localPlayer) {
		//*(int*)((uint64_t)localPlayer + 0x8c8) |= 1024;
	}
}