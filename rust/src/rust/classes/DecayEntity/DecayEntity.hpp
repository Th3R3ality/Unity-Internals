#pragma once
#include "rust/classes/BaseCombatEntity/BaseCombatEntity.hpp"
#include "Il2cppLib.h"

class DecayEntity : public BaseCombatEntity
{
public:
	type_fn("DecayEntity");

    unsigned int buildingID();
};

