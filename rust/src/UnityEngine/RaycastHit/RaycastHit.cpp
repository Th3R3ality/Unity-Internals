#include "RaycastHit.hpp"
#include "Il2cppLib.h"

namespace UnityEngine
{
	Transform* UnityEngine::RaycastHit::transform()
	{
		static auto fn = (Transform * (*)(RaycastHit*))Il2cppLib::method_from_signature("UnityEngine::RaycastHit.get_transform()");
		if (fn)
			return fn(this);
		return nullptr;
	}
}
