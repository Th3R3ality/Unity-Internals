#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

#include "mscorlib/System/String/String.hpp"

namespace UnityEngine
{
	class Object
	{
	public:
		static Object* Instantiate(Object* original, Vector3 pos, Quaternion rot);
		static Object* Instantiate(Object* original);
		static void Destroy(Object* obj);

		mscorlib::System::String* name();

		static Object* FindObjectFromInstanceID(int instanceID);
	};
}