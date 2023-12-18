#include "Component.hpp"

#include "UnityEngine/GameObject/GameObject.hpp"

#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Type/Type.hpp"
using namespace mscorlib;

namespace UnityEngine
{
	Transform* Component::transform()
	{
		static auto fn = (Transform * (*)(Component*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.get_transform()");
		return fn(this);
	}

	GameObject* Component::gameObject()
	{
		static auto fn = (GameObject * (*)(Component*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.get_gameObject()");
		if (fn)
			return fn(this);
		return{ 0 };
	}
	
	Component* Component::GetComponent(System::Type* t) {
		static auto fn = (Component*(*)(Component*, System::Type*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.GetComponent(Type)");
		return fn(this, t);
	}

	System::Array<Component*>* Component::GetComponentsInChildren(System::Type* t)
	{
		static auto fn = (System::Array<Component*>*(*)(Component*, System::Type*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.GetComponentsInChildren(Type)");
		return fn(this, t);
	}
}