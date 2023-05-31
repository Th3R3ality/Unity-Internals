#include "hooks.hpp"

void hk__PWM_HandleJumping(PlayerWalkMovement* instance, ModelState* state, bool wantsJump, bool jumpInDirection)
{
	if (wantsJump)
		instance->Jump(state, jumpInDirection);
}