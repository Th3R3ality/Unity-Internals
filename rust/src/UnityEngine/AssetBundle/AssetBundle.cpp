#include "pch.h"

#include "AssetBundle.hpp"
using namespace mscorlib;

#include "Il2cppLib.h"

namespace UnityEngine
{
	AssetBundle* AssetBundle::LoadFromFile(System::String* path)
	{
		auto fn = (AssetBundle * (*)(System::String*))Il2cppLib::method_from_signature("UnityEngine::AssetBundle.LoadFromFile(string path)");
		
		return { 0 };
		//return fn(path);
	}
}