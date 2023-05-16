#include "Component.hpp"

#include "UnityEngine/GameObject/GameObject.hpp"

#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Type/Type.hpp"
using namespace mscorlib;


namespace UnityEngine
{
	
	GameObject* Component::gameObject()
	{
		static auto fn = (GameObject * (*)(Component*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.get_gameObject()");
		return fn(this);
	}
	
	System::Array<Component*>* Component::GetComponentsInChildren(System::Type* t)
	{
		static auto fn = (System::Array<Component*>*(*)(Component*, System::Type*))Il2cppLib::virtual_method_from_signature("UnityEngine::Component.GetComponentsInChildren(Type)");
		return fn(this, t);
	}
	
}