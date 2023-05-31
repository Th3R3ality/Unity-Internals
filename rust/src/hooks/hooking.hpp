#pragma once
#include <string>
#include <map>

namespace hooking
{
	enum HK_STATUS
	{
		none				= 0,

		not_created			= 1 << 0,
		
		created				= 1 << 1,
		enabled				= 1 << 2,
		disabled			= 1 << 3,

		failed_create		= 1 << 4,
		failed_enable		= 1 << 5,
		failed_disable		= 1 << 6,

		already_created		= 1 << 7,
		already_enabled		= 1 << 8,
		already_disable		= 1 << 9
	};

	std::string get_status_message(HK_STATUS status);
	
	HK_STATUS new_hook(std::string name, void* p_target, void* p_detour);
	HK_STATUS new_hook(std::string name, std::string il2cpp_method_signature, void* p_detour);
	HK_STATUS toggle(std::string name);
	HK_STATUS enable(std::string name);
	HK_STATUS disable(std::string name);
	HK_STATUS status(std::string name);

	void restore_all();
	void* original(std::string name);
}


