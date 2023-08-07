#include "EntityComponent.hpp"
#include "Il2cppLib.h"

/*
template<typename t>
BaseEntity* EntityComponent<t>::GetBaseEntity()
{
	static auto fn = (BaseEntity*(*)(EntityComponent<t>*))Il2cppLib::method_from_signature("EntityComponent.GetBaseEntity()");
	return fn(this);
}
*/