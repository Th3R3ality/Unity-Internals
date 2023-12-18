#pragma once
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/Shader/Shader.hpp"

namespace UnityEngine
{
	class Material : public Object
	{
	public:

		Shader* shader();
	};
}