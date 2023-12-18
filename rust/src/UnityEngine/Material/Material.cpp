#include "Material.hpp"

#include "Il2cppLib.h"

namespace UnityEngine {
	Shader* Material::shader()
	{
		static auto fn = (Shader * (*)(Material*))Il2cppLib::method_from_signature("UnityEngine::Material.get_shader()");
		return fn(this);
	}
}