#pragma once
#include "UnityEngine/Matrix4x4/Matrix4x4.hpp"

namespace UnityEngine
{
	class GL
	{
	public:
		static Matrix4x4 GetGPUProjectionMatrix(Matrix4x4 proj, bool renderIntoTexture);
	};

}