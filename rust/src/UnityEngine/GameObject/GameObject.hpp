#pragma once
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/enums/PrimitiveType.hpp"

#include "UnityEngine/Transform/Transform_proxy.hpp"
#include "UnityEngine/Component/Component_proxy.hpp"

#include "mscorlib/System/Type/Type.hpp"
//using namespace mscorlib;

#include "Il2cppLib.h"

namespace UnityEngine
{
	class GameObject : public Object
	{
	public:

		Transform_proxy* transform();

		/*
		__forceinline operator mscorlib::System::Type* ()
		{
			return (mscorlib::System::Type*)Il2cppLib::type_object("UnityEngine::GameObject");
		}
		*/

		type_overload("UnityEngine::GameObject")

		static GameObject* CreatePrimitive(PrimitiveType type);

		Component_proxy* AddComponent(mscorlib::System::Type* componentType);
	};
}