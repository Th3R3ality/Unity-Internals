#pragma once

namespace mem
{
	template <typename t>
	bool inline validate(t peter)
	{
		if (*reinterpret_cast<uintptr_t*>(&peter) < 0xffffff)
			return false;
		if (*reinterpret_cast<uintptr_t*>(&peter) > 0x7fffffff0000)
			return false;
		return true;
	}
}