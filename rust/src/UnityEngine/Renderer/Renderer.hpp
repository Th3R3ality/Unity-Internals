#pragma once
#include "UnityEngine/Component/Component.hpp"
#include "mscorlib/System/Array/Array.hpp"
#include "UnityEngine/Material/Material.hpp"

namespace UnityEngine
{
	class Renderer : public Component
	{
	public:
		type_overload("UnityEngine::Renderer")

		void enabled(bool value);
		mscorlib::System::Array<Material*>* materials();
	};
}