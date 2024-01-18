#include "Physics.hpp"

#include "Il2cppLib.h"

#include <iostream>

namespace UnityEngine
{
	bool Physics::AutoCast(Vector3 origin, Vector3 direction, int layerMask, float maxDistance, float radius, Vector3 capsuleTop)
	{
		RaycastHit hitInfo;
		Physics::AutoCast(origin, direction, hitInfo, layerMask, maxDistance, radius, capsuleTop);
	}

	bool Physics::AutoCast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, int layerMask, float maxDistance, float radius, Vector3 capsuleTop)
	{
		if (radius == 0)
			return Physics::Raycast(origin, direction, hitInfo, maxDistance, layerMask);
		if (capsuleTop == 0)
			return Physics::SphereCast(origin, radius, direction, hitInfo, maxDistance, layerMask);

		return Physics::CapsuleCast(origin, capsuleTop, radius, direction, hitInfo, maxDistance, layerMask);
	}
	bool UnityEngine::Physics::Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask)
	{
		static auto fn = (bool(*)(Vector3, Vector3, RaycastHit&, float, int))Il2cppLib::method_from_signature("UnityEngine::Physics.Raycast(Vector3, Vector3, UnityEngine.RaycastHit&, float, int)");
		if (fn)
		{
			return fn(origin, direction, hitInfo, maxDistance, layerMask);
		}
		return false;
	}
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
	bool UnityEngine::Physics::Linecast(Vector3 start, Vector3 end, int layerMask)
	{
		RaycastHit hitInfo;
		return Physics::Linecast(start, end, hitInfo, layerMask);
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
	bool UnityEngine::Physics::SphereCast(Vector3 origin, float radius, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask)
	{
		static auto fn = (bool(*)(Vector3, float, Vector3, RaycastHit&, float, int))Il2cppLib::method_from_signature("UnityEngine::Physics.SphereCast(Vector3, float, Vector3, UnityEngine.RaycastHit&, float, int)");
		if (fn)
		{
			return fn(origin, radius, direction, hitInfo, maxDistance, layerMask);
		}
		return false;
	}
	bool UnityEngine::Physics::CapsuleCast(Vector3 point1, Vector3 point2, float radius, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask)
	{
		static auto fn = (bool(*)(Vector3, Vector3, float, Vector3, RaycastHit&, float, int))Il2cppLib::method_from_signature("UnityEngine::Physics.SphereCast(Vector3, float, Vector3, UnityEngine.RaycastHit&, float, int)");
		if (fn)
		{
			return fn(point1, point2, radius, direction, hitInfo, maxDistance, layerMask);
		}
		return false;
	}
}
