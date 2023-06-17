#include "hooks.hpp"

#include <iostream>
#include <Windows.h>
#include <cheat.hpp>


void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{
	static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
		std::cout << orig << " : orig much???" << std::endl;
		return;
	}
	orig(instance);


	if (cheat::state() == cheat::status::unloading) {
		//cheat::unload_gameObjects();
		//cheat::unload_assetbundles();
		cheat::has_unloaded(true);
	}

}