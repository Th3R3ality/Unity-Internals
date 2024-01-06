#include "BaseProjectile.hpp"
#include "Il2cppLib.h"

#include <iostream>
#include <format>

mscorlib::System::List<Projectile*>* BaseProjectile::createdProjectiles()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BaseProjectile.createdProjectiles");
	if (offset) {
		std::cout << std::format("this : {:x}\n", (uintptr_t)this);
		std::cout << std::format("offset : {:x}\n", offset);
		return *(mscorlib::System::List<Projectile*>**)((uintptr_t)this + offset);
	}
	return nullptr;
}
