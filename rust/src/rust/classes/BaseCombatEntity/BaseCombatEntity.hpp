#pragma once
#include "rust/classes/BaseEntity/BaseEntity.hpp"

class BaseCombatEntity : public BaseEntity
{
public:
	enum class LifeState
	{
		Alive,
		Dead
	};
	
	LifeState lifestate();
	void lifestate(LifeState state);

};