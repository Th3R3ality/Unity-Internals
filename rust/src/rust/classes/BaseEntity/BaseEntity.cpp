#include "BaseEntity.hpp"

#include "Il2cppLib.h"

#include <iostream>

Model* BaseEntity::model()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BaseEntity.model");
	std::cout << "BaseEntity.model offset: " << offset << std::endl;
	std::cout << "Instance*: " << this << std::endl;
	std::cout << "Model* : " << *(Model**)((uintptr_t)this + offset) << std::endl;

	return *(Model**)((uintptr_t)this + offset);
}