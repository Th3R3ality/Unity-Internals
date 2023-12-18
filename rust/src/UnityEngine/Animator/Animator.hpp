#pragma once

#include "UnityEngine/Behaviour/Behaviour.hpp"
#include "UnityEngine/Avatar/Avatar.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	class Animator : public Behaviour
	{
	public:
		type_overload("UnityEngine::Animator");

		Avatar* avatar();
		void avatar(Avatar* avatar);
	};
}