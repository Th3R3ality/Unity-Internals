#pragma once

#include "Alias.h"
#include "Goal.h"

namespace RustBot
{
	enum class BotState
	{
		idle,
		pathing,
		walking
	};
	
	enum Resource
	{
		wood = 1 << 0,
		stones = 1 << 1,
		metal = 1 << 2,
		sulfur = 1 << 3,
		barrels = 1 << 4,
		crates = 1 << 5,
		collectibles = 1 << 6,

		nodes = stones | metal | sulfur,
		scrap = barrels | crates,
	};

	BotState GetState();

	void Goto(float x, float y, float z);
	void Goto(float x, float z);
	void Goto(float y);

	void SetGoal(float x, float y, float z);
	void SetGoal(float x, float z);
	void SetGoal(float y);

	void Go();

	void Invert();

	void Cancel();
	inline void Stop() { Cancel(); }

	void Gather(int resourceMask);
}