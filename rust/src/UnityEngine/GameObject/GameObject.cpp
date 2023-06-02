#include "GameObject.hpp"

namespace UnityEngine
{
	Transform* GameObject::transform()
	{
		static auto fn = (Transform * (*)(GameObject*))Il2cppLib::method_from_signature("UnityEngine::GameObject.get_transform()");
		return fn(this);
	}

	GameObject* GameObject::CreatePrimitive(PrimitiveType type)
	{
		static auto fn = (GameObject* (*)(PrimitiveType))Il2cppLib::method_from_signature("UnityEngine::GameObject.CreatePrimitive(UnityEngine.PrimitiveType)");
		return fn(type);
	}
	
	Component* GameObject::AddComponent(mscorlib::System::Type* componentType)
	{
		static auto fn = (Component*(*)(GameObject*, mscorlib::System::Type*))Il2cppLib::method_from_signature("UnityEngine::GameObject.AddComponent(Type)");
		return fn(this, componentType);
	}
	
	mscorlib::System::Array<Component*>* GameObject::GetComponentsInChildren(mscorlib::System::Type* type)
	{
		static auto fn = (mscorlib::System::Array<Component*>*(*)(GameObject*, mscorlib::System::Type*))Il2cppLib::method_from_signature("UnityEngine::GameObject.GetComponentsInChildren(Type)");
		return fn(this, type);
	}

	void GameObject::SetActive(bool value)
	{
		static auto fn = (void(*)(GameObject*, bool))Il2cppLib::method_from_signature("UnityEngine::GameObject.SetActive(bool)");
		return fn(this, value);
	}
}

