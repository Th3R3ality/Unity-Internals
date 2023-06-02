#pragma once
#include <string>

#include "cache.hpp"
#include "Il2cppLib.h"
#include "UnityEngine/UnityEngine.hpp"
#include "mscorlib/mscorlib.hpp"

namespace cheat
{
	enum status
	{
		none,
		loading,
		running,
		unloading
	};

	status state();
	void state(status new_state);

	bool has_unloaded();
	void has_unloaded(bool status);

	void unload_gameObjects();
	void unload_assetbundles();
	
	UnityEngine::AssetBundle* load_assetbundle(std::string path);

	System::Array<System::String*>* get_asset_names();

	bool init_hooks();

	void unload();
}