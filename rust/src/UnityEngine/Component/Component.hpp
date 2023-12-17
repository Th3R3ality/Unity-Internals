#pragma once
#include "UnityEngine/Object/Object.hpp"

#include "UnityEngine/GameObject/GameObject.hpp"

#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Type/Type.hpp"
//using namespace mscorlib;


namespace UnityEngine
{
	class Transform;
	class Component : public Object
	{
	public:
		Transform* transform();

		GameObject* gameObject();

		mscorlib::System::Array<Component*>* GetComponentsInChildren(mscorlib::System::Type* t);

	};
}