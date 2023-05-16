#include "Object.hpp"
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	Object* Object::Instantiate(Object* original)
	{
		static auto fn = (Object* (*)(Object *))Il2cppLib::virtual_method_from_signature("UnityEngine::Object.Instantiate(Object)");
		return fn(original);
	}
}