#pragma once
#include "rust/classes/AttackEntity/AttackEntity.hpp"
#include "mscorlib/System/List/List.hpp"
#include "rust/classes/Projectile/Projectile.hpp"

class BaseProjectile : public AttackEntity
{
public:
	mscorlib::System::List<Projectile*>* createdProjectiles();
};