#pragma once

//automatic extern defines
#ifndef GSMDEF
#define GSMDEF(x) extern x
#endif

namespace StateMachine
{
	GSMDEF(bool doJump);
}