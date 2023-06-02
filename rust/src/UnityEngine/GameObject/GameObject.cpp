#include "GameObject.hpp"

namespace UnityEngine
{
	Transform_proxy* GameObject::transform()
	{
		static auto fn = (Transform_proxy * (*)(GameObject*))Il2cppLib::method_from_signature("UnityEngine::GameObject.get_transform()");
		return fn(this);
	}

	GameObject* GameObject::CreatePrimitive(PrimitiveType type)
	{
		static auto fn = (GameObject* (*)(PrimitiveType))Il2cppLib::method_from_signature("UnityEngine::GameObject.CreatePrimitive(UnityEngine.PrimitiveType)");
		return fn(type);
	}
	
	Component_proxy* GameObject::AddComponent(mscorlib::System::Type* componentType)
	{
		static auto fn = (Component_proxy*(*)(GameObject*, mscorlib::System::Type*))Il2cppLib::method_from_signature("UnityEngine::GameObject.AddComponent(Type)");
		return fn(this, componentType);
	}
	
}