#pragma once
#include "UnityEngine/Component/Component.hpp"

namespace UnityEngine
{
	class Renderer : public Component
	{
	public:
		type_overload("UnityEngine::Renderer")

		void enabled(bool value);
	};
}