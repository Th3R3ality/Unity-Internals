#pragma once
#include "UnityEngine/Object/Object.hpp"

//#include "UnityEngine/Transform/Transform.hpp"

#include "mscorlib/System/Type/Type.hpp"
//using namespace mscorlib;

#include "Il2cppLib.h"

namespace UnityEngine
{
	class GameObject : public Object
	{
	public:

		//Transform* transform();

		__forceinline operator mscorlib::System::Type* ()
		{
			return (mscorlib::System::Type*)Il2cppLib::type_object("UnityEngine::GameObject");
		}
	};
}