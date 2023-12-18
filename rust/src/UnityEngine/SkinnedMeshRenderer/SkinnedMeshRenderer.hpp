#pragma once
#include "UnityEngine/Renderer/Renderer.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	class SkinnedMeshRenderer : public Renderer
	{
	public:
		type_overload("UnityEngine::SkinnedMeshRenderer");

		Transform* rootBone();
	};
}