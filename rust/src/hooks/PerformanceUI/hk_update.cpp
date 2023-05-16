#include "hk_update.hpp"

#include "hooks/hooking.hpp"

#include <iostream>

void hk__PerformanceUI_Update(Facepunch::PerformanceUI* instance)
{
	static auto orig = hooking::original(hk__PerformanceUI_Update_sig);
	if (orig)
		((decltype(&hk__PerformanceUI_Update))*orig)(instance);

	std::cout << "pu_update hook!" << std::endl;
}