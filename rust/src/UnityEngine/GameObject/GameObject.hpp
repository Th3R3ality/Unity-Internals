#pragma once
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/enums/PrimitiveType.hpp"

#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Type/Type.hpp"
//using namespace mscorlib;

#include "Il2cppLib.h"

namespace UnityEngine
{
	class Transform;
	class Component;
	class GameObject : public Object
	{
	public:

		type_overload("UnityEngine::GameObject")

		static GameObject* CreatePrimitive(PrimitiveType type);

		Transform* transform();
		Component* AddComponent(mscorlib::System::Type* componentType);
		mscorlib::System::Array<Component*>* GetComponentsInChildren(mscorlib::System::Type* type);
		void SetActive(bool value);

	};
}