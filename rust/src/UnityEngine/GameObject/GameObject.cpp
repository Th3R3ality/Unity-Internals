#include "GameObject.hpp"

#include "UnityEngine/Transform/Transform_proxy.hpp"

namespace UnityEngine
{
	Transform_proxy* GameObject::transform()
	{
		static auto fn = (Transform_proxy * (*)(GameObject*))Il2cppLib::virtual_method_from_signature("UnityEngine::GameObject.get_transform()");
		return fn(this);
	}
}