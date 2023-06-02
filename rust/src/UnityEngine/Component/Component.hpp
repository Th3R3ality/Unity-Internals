#pragma once
#include "UnityEngine/Object/Object.hpp"

#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/Transform/Transform_proxy.hpp"

#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Type/Type.hpp"
//using namespace mscorlib;

#define _component(x) ((UnityEngine::Component*)x)

namespace UnityEngine
{
	class Component : public Object
	{
	public:
		Transform_proxy* transform();

		GameObject* gameObject();

		mscorlib::System::Array<Component*>* GetComponentsInChildren(mscorlib::System::Type* t);
	};
}