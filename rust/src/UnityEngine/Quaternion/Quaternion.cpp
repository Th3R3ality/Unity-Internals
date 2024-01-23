#include "Quaternion.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	Quaternion Quaternion::identity()
	{
		static auto fn = (Quaternion(*)())Il2cppLib::method_from_signature("UnityEngine::Quaternion.get_identity()");
		return fn();
	}

	Quaternion Quaternion::Euler(Vector3 euler)
	{
		static auto fn = (Quaternion(*)(Vector3))Il2cppLib::method_from_signature("UnityEngine::Quaternion.Euler(Vector3)");
		if (fn)
			return fn(euler);

		return {};
	}


}