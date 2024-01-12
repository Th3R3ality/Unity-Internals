#include "Physics.hpp"

#include "Il2cppLib.h"

#include <iostream>

namespace UnityEngine
{
	bool Physics::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance)
	{
		static auto fn = (bool(*)(Vector3, Vector3, RaycastHit&, float))Il2cppLib::method_from_signature("UnityEngine::Physics.Raycast(Vector3, Vector3, UnityEngine.RaycastHit&, float)");
		if (fn)
		{
			return fn(origin, direction, hitInfo, maxDistance);
		}
		return false;
	}
	bool Physics::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo)
	{
		static auto fn = (bool(*)(Vector3, Vector3, RaycastHit&))Il2cppLib::method_from_signature("UnityEngine::Physics.Raycast(Vector3, Vector3, UnityEngine.RaycastHit&)");
		if (fn) {
			return fn(origin, direction, hitInfo);
		}
		return false;
	}
}
