#include "Object.hpp"
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

#include "Il2cppLib.h"

#include "mscorlib/System/String/String.hpp"
using namespace mscorlib;

namespace UnityEngine
{
	Object* Object::Instantiate(Object* original, Vector3 pos, Quaternion rot)
	{
		static auto fn = (Object* (*)(Object *, Vector3, Quaternion))Il2cppLib::method_from_signature("UnityEngine::Object.Instantiate(Object, Vector3, Quaternion)");
		return fn(original, pos, rot);
	}

	Object* Object::Instantiate(Object* original)
	{
		static auto fn = (Object * (*)(Object*))Il2cppLib::method_from_signature("UnityEngine::Object.Instantiate(Object)");
		return fn(original);
	}

	void Object::Destroy(Object* obj)
	{
		static auto fn = (void(*)(Object*))Il2cppLib::method_from_signature("UnityEngine::Object.Destroy(Object)");
		return fn(obj);
	}

	System::String* Object::name()
	{
		static auto fn = (System::String* (*)(Object *))Il2cppLib::method_from_signature("UnityEngine::Object.get_name()");
		if (fn)
			return fn(this);
		return { 0 };
	}
}