
#include "AssetBundle.hpp"
using namespace mscorlib;

#include "Il2cppLib.h"

namespace UnityEngine
{
	AssetBundle* AssetBundle::LoadFromFile(System::String* path)
	{
		static auto fn = (AssetBundle* (*)(System::String*))Il2cppLib::virtual_method_from_signature("UnityEngine::AssetBundle.LoadFromFile(string)");
		return fn(path);
	}

	System::Array<System::String*>* AssetBundle::GetAllAssetNames()
	{
		static auto fn = (System::Array<System::String*>*(*)(AssetBundle*))Il2cppLib::virtual_method_from_signature("UnityEngine::AssetBundle.GetAllAssetNames()");
		return fn(this);
	}

	UnityEngine::Object* AssetBundle::LoadAsset(std::string name, System::Type* type)
	{
		static auto fn = (UnityEngine::Object* (*)(UnityEngine::AssetBundle*, System::String*, System::Type*))Il2cppLib::virtual_method_from_signature("UnityEngine::AssetBundle.LoadAsset(string, Type)");
		return fn(this, Il2cppLib::api::new_string<System::String *>(name), type);
	}
}