#include "cheat.hpp"

#include <string>
#include <iostream>

#include "minhook/include/MinHook.h"

#include "hooks/hooking2.hpp"
#include "hooks/hooks.hpp"

#include "UnityEngine/UnityEngine.hpp"



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

		//hooking::New("pu_update", Il2cppLib::method_from_signature(hk__FP_PU_Update_sig), (void*)hk__FP_PU_Update);
		//hooking::Enable("pu_update");

		HOOK(FP_PU_Update);

		HOOK(BP_Load);

		HOOK(PWM_HandleJumping);

		HOOK(HE_AddPunch);

		/*
		hooking::HK_STATUS status;

		status = hooking::new_hook("bp_load", hk__BP_Load_sig, &hk__BasePlayer_Load);

		std::cout << "hk_create | bp_load: " << hooking::get_status_message(status) << std::endl;
		std::cout << "hk_enable | bp_load: " << hooking::get_status_message(hooking::enable("bp_load")) << std::endl;


		//status = hooking::new_hook("pu_update", hk__FP_PU_Update_sig, &hk__FP_PU_Update);

		//std::cout << "hk_create | pu_update: " << hooking::get_status_message(status) << std::endl;
		//std::cout << "hk_enable | pu_update: " << hooking::get_status_message(hooking::enable("pu_update")) << std::endl;
		*/

		return true;
	}

	void unload()
	{
		std::cout << "unloading" << std::endl;

		hooking::Kill();
		//hooking::restore_all();
		MH_Uninitialize();
	}
}