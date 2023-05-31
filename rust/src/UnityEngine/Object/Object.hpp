#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

#include "mscorlib/System/String/String.hpp"

namespace UnityEngine
{
	class Object
	{
	public:
		static Object* Instantiate(Object* original);

		mscorlib::System::String* name();
	};
}