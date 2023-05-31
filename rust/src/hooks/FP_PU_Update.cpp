#include "hooks.hpp"

#include <iostream>

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{
	static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
			std::cout << orig << " : orig much???" << std::endl;
		return;
	}

	orig(instance);

	if (counter % 5000 == 0)
		std::cout << "pu_update hook!" << std::endl;
}