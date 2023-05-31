#pragma once
#include <string>
#include <unordered_map>

namespace hooking
{
	extern std::unordered_map<std::string, void*> __origs;
	extern std::unordered_map<std::string, void*> __targets;

	void New(std::string __name, void* __target, void* __detour);

	void* Original(std::string __name);
	void* Original(void* __detour);

	void Enable(std::string __name);
	void Kill();
}