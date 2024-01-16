#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"

namespace UnityEngine
{
	class Physics
	{
	public:
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance, int layerMask);
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo, float maxDistance);
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo);
		static bool Linecast(Vector3 start, Vector3 end);
		static bool Linecast(Vector3 start, Vector3 end, int layerMask);
		static bool Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo);
		static bool Linecast(Vector3 start, Vector3 end, RaycastHit& hitInfo, int layerMask);
	};

}