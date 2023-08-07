#pragma once
#include "rust/classes/EntityComponentBase/EntityComponentBase.hpp"
#include "rust/classes/BaseEntity/BaseEntity.hpp"

template <typename T>
class EntityComponent : public EntityComponentBase
{
public:
    static_assert(std::is_base_of<BaseEntity, T>::value, "T must be a derived type of BaseEntity");

	BaseEntity* GetBaseEntity()
	{
		static auto fn = (BaseEntity*(*)(EntityComponent<T>*))Il2cppLib::method_from_signature("EntityComponent.GetBaseEntity()");
		return fn(this);
	}
};