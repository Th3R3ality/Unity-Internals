#include "GL.hpp"
#include "Il2cppLib.h"

namespace UnityEngine
{
	Matrix4x4 GL::GetGPUProjectionMatrix(Matrix4x4 proj, bool renderIntoTexture)
	{
		auto fn = ((Matrix4x4(*)(Matrix4x4, bool))Il2cppLib::method_from_signature("UnityEngine::GL.GetGPUProjectionMatrix(Matrix4x4, bool)"));
		if (fn)
			return fn(proj, renderIntoTexture);
		return {};
	}
}
