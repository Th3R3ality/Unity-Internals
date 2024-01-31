#pragma once

#include "Alias.h"

namespace RustBot
{
	enum class GoalType
	{
		none,
		//Exact 3D position
		exact,
		//Beam-like goal, height doesnt affect cost
		horizontal,
		//Plane-like goal, get as close vertical as possible (pretty useless)
		vertical,
		//Get as far away from the goal
		exactInverted,
		//Get as far away from the goal horizontally
		horizontalInverted,
		//Get as far away from the goal vertically
		verticalInverted
	};

	class Goal
	{
	public:
		Goal() = default;

		Goal(GoalType type, v3 goalPosition) :
			type(type), pos(goalPosition)
		{}

		float Heuristic(v3 pos);
		v3 Position(v3 nodePos = 0);

	private:
		GoalType type = GoalType::none;
		v3 pos{};

		float HeuristicExact(v3 pos);
		float HeuristicHorizontal(v3 pos);
		float HeuristicVertical(v3 pos);
		float HeuristicExactInverted(v3 pos);
		float HeuristicHorizontalInverted(v3 pos);
		float HeuristicVerticalInverted(v3 pos);

	};
}