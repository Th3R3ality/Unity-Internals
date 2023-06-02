#include "AsyncOperations.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	bool AsyncOperation::isDone()
	{
		static auto fn = (bool(*)(AsyncOperation*))Il2cppLib::method_from_signature("UnityEngine::AsyncOperation.get_isDone()");
		return fn(this);
	}
}