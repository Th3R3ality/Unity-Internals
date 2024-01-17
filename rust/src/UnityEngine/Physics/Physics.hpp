#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"

namespace UnityEngine
{
	class Physics
	{
	public:
		static bool AutoCast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, int layerMask, float maxDistance = INFINITY, float radius = 0, Vector3 height = 0);
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask);
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance);
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo);
		static bool Linecast(Vector3 start, Vector3 end);
		static bool Linecast(Vector3 start, Vector3 end, int layerMask);
		static bool Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo);
		static bool Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo, int layerMask);
		static bool SphereCast(Vector3 origin, float radius, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask);
		static bool CapsuleCast(Vector3 point1, Vector3 point2, float radius, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask);
	};

}