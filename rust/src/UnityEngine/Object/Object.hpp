#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

namespace UnityEngine
{
	class Object
	{
	public:
		static Object* Instantiate(Object* original);

	};
}