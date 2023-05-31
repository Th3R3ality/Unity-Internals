#include "hooking2.hpp"

#include <unordered_map>
#include <memory>

#include "minhook/include/MinHook.h"

#include "Il2cppLib.h"
#include <iostream>

namespace hooking
{
	std::unordered_map<std::string, void*> __origs;
	std::unordered_map<std::string, void*> __targets;

	std::unordered_map<void*, std::string> __googoo_gaga;

	void New(std::string __name, void* __target, void* __detour){
		std::cout << "__target: " << __target << std::endl;

		__origs[__name] = 0x0;
		__googoo_gaga[__detour] = __name;
		auto __status = MH_CreateHook(__target, __detour, &__origs[__name]);
		std::cout << "hook create - " << __name << " : " << MH_StatusToString(__status) << std::endl;

		std::cout << "orig : " << Original(__name) << std::endl;

		if (__status == MH_OK)
			__targets[__name] = __target;
	}

	void* Original(std::string __name)
	{
		return __origs[__name];
	}

	void* Original(void* __detour)
	{
		return __origs[__googoo_gaga[__detour]];
	}

	void Enable(std::string __name)
	{
		auto __status = MH_EnableHook(__targets[__name]);
		std::cout << "hook enable - " << __name << " : " << MH_StatusToString(__status) << std::endl;
	}

	void Kill()
	{
		MH_DisableHook(MH_ALL_HOOKS);
		//MH_RemoveHook();
	}
}