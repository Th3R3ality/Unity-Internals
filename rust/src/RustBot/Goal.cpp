#include "Goal.h"

namespace RustBot
{
	float Goal::Heuristic(v3 nodePos)
	{
		switch (type)
		{
		case GoalType::exact:
			return HeuristicExact(nodePos);
		case GoalType::horizontal:
			return HeuristicHorizontal(nodePos);
		case GoalType::vertical:
			return HeuristicVertical(nodePos);
		case GoalType::exactInverted:
			return HeuristicExactInverted(nodePos);
		case GoalType::horizontalInverted:
			return HeuristicHorizontalInverted(nodePos);
		case GoalType::verticalInverted:
			return HeuristicVerticalInverted(nodePos);
		default:
			break;
		}

		return 0.0f;
	}

	v3 Goal::Position(v3 nodePos)
	{
		switch (type)
		{
		case GoalType::exact:
			return pos;
		case GoalType::horizontal:
			return v3(pos.x, nodePos.y, pos.z);
		case GoalType::vertical:
			return v3(nodePos.x, pos.y, nodePos.z);
		case GoalType::exactInverted:
			return v3::Normalize(pos - nodePos) * -1000;
		case GoalType::horizontalInverted:
			return v3::Normalize(v3(pos.x - nodePos.x, nodePos.y, pos.z - nodePos.z)) * -100;
		case GoalType::verticalInverted:
			return v3::Normalize(v3(nodePos.x, pos.y - nodePos.y, nodePos.z)) * -100;
		default:
			break;
		}

		return v3(0.0f);
	}


	float Goal::HeuristicExact(v3 nodePos)
	{
		return v3::Distance(pos, nodePos);
	}
	float Goal::HeuristicHorizontal(v3 nodePos)
	{
		return v3::Distance(v3(pos.x, nodePos.y, pos.z), nodePos);
	}
	float Goal::HeuristicVertical(v3 nodePos)
	{
		return v3::Distance(v3(nodePos.x, pos.y, nodePos.z), nodePos);
	}
	float Goal::HeuristicExactInverted(v3 nodePos)
	{
		return -HeuristicExact(nodePos);
	}
	float Goal::HeuristicHorizontalInverted(v3 nodePos)
	{
		return -HeuristicHorizontal(nodePos);
	}
	float Goal::HeuristicVerticalInverted(v3 nodePos)
	{
		return -HeuristicVertical(nodePos);
	}
}
