#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"

namespace UnityEngine
{
	class Physics
	{
	public:
		static bool Raycast(Vector3 origin, Vector3 direction, RaycastHit& hitInfo);
	};

}