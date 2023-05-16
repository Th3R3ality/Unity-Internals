#include "cheat.hpp"

#include <string>
#include <iostream>

#include "hooks/hooking.hpp"
#include "hooks/hooks.hpp"

#include "hooks/BasePlayer/hk_load.hpp"

#include "UnityEngine/UnityEngine.hpp"

#include "minhook/include/MinHook.h"


namespace cheat
{
	UnityEngine::AssetBundle* bundle = 0;
	UnityEngine::GameObject* prefab = 0;
	bool load_assets()
	{
		bundle = UnityEngine::AssetBundle::LoadFromFile(Il2cppLib::api::new_string<System::String*>("C:\\Users\\reality\\Desktop\\monke.bundle"));
		if (!bundle) return false;
		UnityEngine::Object* prefab = bundle->LoadAsset("assets/monke.prefab", UnityEngine::GameObject());
		if (!prefab) return false;
		return true;
	}

	UnityEngine::GameObject* instantiate_prefab()
	{
		return (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
	}

	System::Array<System::String*>* get_asset_names()
	{
		return bundle->GetAllAssetNames();
	}

	bool init_hooks()
	{
		MH_Initialize();

		hooking::HK_STATUS status;

		//status = hooking::new_hook("bp_load", hk__BasePlayer_Load_sig, &hk__BasePlayer_Load);
		//std::cout << "hk | bp_load: " << hooking::get_status_message(status) << std::endl;

		status = hooking::new_hook("pu_update", hk__PerformanceUI_Update_sig, &hk__PerformanceUI_Update);
		std::cout << "hk_create | pu_update: " << hooking::get_status_message(status) << std::endl;

		std::cout << "hk_enable | pu_update: " << hooking::get_status_message(hooking::enable("pu_update")) << std::endl;

		return true;
	}

	void unload()
	{
		std::cout << "unloading" << std::endl;

		hooking::restore_all();
		MH_Uninitialize();
	}
}