
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

	Object* AssetBundle::LoadAsset(std::string name, System::Type* type)
	{
		static auto fn = (Object* (*)(AssetBundle*, System::String*, System::Type*))Il2cppLib::method_from_signature("UnityEngine::AssetBundle.LoadAsset_Internal(string, Type)");
		return fn(this, Il2cppLib::api::new_string<System::String *>(name), type);
	}

	AssetBundleRequest* AssetBundle::LoadAssetAsync(std::string name, System::Type* type)
	{
		static auto fn = (AssetBundleRequest * (*)(AssetBundle*, System::String*, System::Type*))Il2cppLib::method_from_signature("UnityEngine::AssetBundle.LoadAssetAsync_Internal(string, Type)");
		return fn(this, Il2cppLib::api::new_string<System::String*>(name), type);
	}

	void AssetBundle::Unload(bool unloadAllLoadedObjects)
	{
		//static auto fn = (void(*)(AssetBundle*, bool))Il2cppLib::method_from_signature("UnityEngine::AssetBundle.Unload(bool)");
		static auto fn = Il2cppLib::resolve_icall("UnityEngine.AssetBundle::Unload(System.Boolean)");
		return ((void(*)(AssetBundle*, bool))fn)(this, unloadAllLoadedObjects);
	}
}