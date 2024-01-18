#include "hooks.hpp"
#include "globalSM.h"

void hk__PWM_HandleJumping(PlayerWalkMovement* instance, ModelState* state, bool wantsJump, bool jumpInDirection)
{
	ORIG(hk__PWM_HandleJumping);
	if (wantsJump)
	{
		instance->Jump(state, jumpInDirection);
	}

	if (StateMachine::doJump)
	{
		StateMachine::doJump = false;
		wantsJump = true;
		orig(instance, state, wantsJump, jumpInDirection);
	}
}