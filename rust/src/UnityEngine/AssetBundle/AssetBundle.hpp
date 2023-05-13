#pragma once
#include "../UnityEngine.hpp"
#include "../../mscorlib/mscorlib.hpp"
using namespace mscorlib;

namespace UnityEngine
{
	class AssetBundle : public Object
	{
	public:
		static AssetBundle* LoadFromFile(System::String* path);
	};
}