#include "RustBot.h"

#include "Pathing.h"
#include "cache.hpp"

namespace RustBot
{
	int drawPathId = 0;
	GoalType goalType = GoalType::none;
	v3 goalPos = 0;
	
	BotState state;
	Pather pather;

	BotState GetState()
	{
		return state;
	}

	void Goto(float x, float y, float z)
	{
		goalType = GoalType::exact;
		goalPos = v3(x, y, z);
		Stop();
		Go();
	}

	void Goto(float x, float z)
	{
		goalType = GoalType::horizontal;
		goalPos = v3(x, 0, z);
		Stop();
		Go();
	}

	void Goto(float y)
	{
		goalType = GoalType::vertical;
		goalPos = v3(0, y, 0);
		Stop();
		Go();
	}

	void SetGoal(float x, float y, float z)
	{
		goalType = GoalType::exact;
		goalPos = v3(x, y, z);
		Stop();
	}

	void SetGoal(float x, float z)
	{
		goalType = GoalType::horizontal;
		goalPos = v3(x, 0, z);
		Stop();
	}

	void SetGoal(float y)
	{
		goalType = GoalType::vertical;
		goalPos = v3(0, y, 0);
		Stop();
	}

	void Go()
	{
		if (goalType == GoalType::none)
			return;

		state = BotState::pathing;
		pather.New(cache::local()->transform()->position(), goalPos, goalType);
	}

	void Invert()
	{
		switch (goalType)
		{
		case GoalType::exact:
			goalType = GoalType::exactInverted;
			break;
		case GoalType::horizontal:
			goalType = GoalType::horizontalInverted;
			break;
		case GoalType::vertical:
			goalType = GoalType::verticalInverted;
			break;
		case GoalType::exactInverted:
			goalType = GoalType::exact;
			break;
		case GoalType::horizontalInverted:
			goalType = GoalType::horizontal;
			break;
		case GoalType::verticalInverted:
			goalType = GoalType::vertical;
			break;
		default:
			break;
		}

		if (state != BotState::idle)
		{
			Cancel();
			Go();
		}
	}

	void Cancel()
	{
		drawPathId = 0;
		pather.Clean();
		state = BotState::idle;
	}

	void DrawPath()
	{
		auto points = pather.GetPathPoints();

		v3 lastPoint = 0;
		for (v3 point : points)
		{
			if (lastPoint != 0)
			{
				cache::debugDraw("path" + std::to_string(drawPathId), cache::debugLine3d(lastPoint, point, (state == BotState::walking) ? "00aa00" : "0000aa"));
			}
			lastPoint = point;
		}
	}

	void Gather(int resourceMask)
	{

	}


}