#pragma once
#include "rust/classes/BaseMovement/BaseMovement.hpp"

#include "rust/classes/ModelState/ModelState.hpp"

#include "Il2cppLib.h"

class PlayerWalkMovement : public BaseMovement
{
public:
	void Jump(ModelState* state, bool inDirection)
	{
		//IL2CPP_METHOD(Jump_fn, "PlayerWalkMovement.Jump", void(*)(PlayerWalkMovement*, ModelState*, bool), (this, state, inDirection));
		static auto fn = (void(*)(PlayerWalkMovement*, ModelState*, bool))Il2cppLib::method_from_signature("PlayerWalkMovement.Jump(ModelState, bool)");
		fn(this, state, inDirection);
	}
};