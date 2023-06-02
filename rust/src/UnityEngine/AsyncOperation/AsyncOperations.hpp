#pragma once
#include "UnityEngine/YieldInstruction/YieldInstruction.hpp"

namespace UnityEngine
{
	class AsyncOperation : public YieldInstruction
	{
	public:
		bool isDone();
	};
}