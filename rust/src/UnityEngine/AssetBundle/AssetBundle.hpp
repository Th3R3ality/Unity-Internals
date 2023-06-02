#pragma once
#include "UnityEngine/Object/Object.hpp"
#include "mscorlib/mscorlib.hpp"
using namespace mscorlib;

#include "UnityEngine/AssetBundleRequest/AssetBundleRequest.hpp"

namespace UnityEngine
{
	class AssetBundle : public Object
	{
	public:
		static AssetBundle* LoadFromFile(System::String* path);

		System::Array<System::String*>* GetAllAssetNames();

		UnityEngine::Object* LoadAsset(std::string name, System::Type* type);
		
		UnityEngine::AssetBundleRequest* LoadAssetAsync(std::string name, System::Type* type);

		void Unload(bool unloadAllLoadedObjects);
	};
}