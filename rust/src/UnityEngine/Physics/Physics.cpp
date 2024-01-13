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
	bool Physics::Linecast(Vector3 start, Vector3 end)
	{
		RaycastHit hitInfo;
		return Physics::Linecast(start, end, hitInfo, -5);
	}
	bool Physics::Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo)
	{
		return Physics::Linecast(start, end, hitInfo, -5);
	}
	bool Physics::Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo, int layerMask)
	{
		static auto fn = (bool(*)(Vector3, Vector3, RaycastHit&, int))Il2cppLib::method_from_signature("UnityEngine::Physics.Linecast(Vector3, Vector3, UnityEngine.RaycastHit&, int)");
		if (fn)
		{
			return fn(start, end, hitInfo, layerMask);
		}
		return false;
	}
}
