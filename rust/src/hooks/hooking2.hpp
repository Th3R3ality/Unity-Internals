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

#include <Windows.h>
inline uintptr_t VMTEntryHook(uintptr_t VMT, size_t Index, uintptr_t Destination)
{
	uintptr_t* Address = (uintptr_t*)(VMT + Index * sizeof(uintptr_t));

	DWORD OldProtection{ 0 };
	VirtualProtect(Address, sizeof(uintptr_t), PAGE_READWRITE, &OldProtection);
	uintptr_t result = *Address;
	*Address = Destination;
	VirtualProtect(Address, sizeof(uintptr_t), OldProtection, &OldProtection);
	return result;
}