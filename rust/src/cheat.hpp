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

	void revert_all_models();
	void revert_model_change(cache::CachedPlayer player);
	void unload_assetbundles();

	UnityEngine::AssetBundle* load_assetbundle(std::string path);

	bool init_hooks();

	void unload();
}