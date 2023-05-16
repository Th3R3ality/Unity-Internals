#pragma once
#include <string>

#include "Il2cppLib.h"
#include "UnityEngine/UnityEngine.hpp"
#include "mscorlib/mscorlib.hpp"

namespace cheat
{
	bool load_assets();
	UnityEngine::GameObject* instantiate_prefab();
	System::Array<System::String*>* get_asset_names();

	bool init_hooks();

	void unload();
}