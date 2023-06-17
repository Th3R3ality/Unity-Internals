#pragma once
#include "rust/classes/BaseCombatEntity/BaseCombatEntity.hpp"
#include "mscorlib/System/String/String.hpp"
using namespace mscorlib;

class BasePlayer : public BaseCombatEntity
{
public:
	System::String* displayName();
};