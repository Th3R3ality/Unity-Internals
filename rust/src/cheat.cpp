#include "cheat.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "minhook/include/MinHook.h"

#include "hooks/hooking2.hpp"
#include "hooks/hooks.hpp"

#include "UnityEngine/UnityEngine.hpp"

#include "cache.hpp"

namespace cheat
{
	status current_state = status::none;
	bool did_unload = false;

	status state()
	{
		return current_state;
	}
	void state(status new_state)
	{
		current_state = new_state;
	}

	bool has_unloaded()
	{
		return did_unload;
	}
	void has_unloaded(bool status)
	{
		did_unload = status;
	}

	void unload_gameObjects()
	{
		auto gameObjects = cache::gameObjects();
		for (auto go : gameObjects) {
			std::cout << "destroying " << go.second << "    key : " << go.first << std::endl;
			UnityEngine::Object::Destroy(go.second);
		}
		gameObjects.clear();
	}

	void revert_model_changes()
	{
		std::cout << "cleaning up model changes\n";
		auto cachedPlayers = cache::cachedPlayers();

		for (auto& cachedPlayer : cachedPlayers) {
			auto animator = cachedPlayer.second.pAnimator;
			std::cout << "animator: " << animator << "\n";
			if (!animator) continue;
			auto avatar = cachedPlayer.second.pOrigAvatar;
			std::cout << "avatar: " << avatar << "\n";
			if (!avatar) continue;

			animator->avatar(avatar);
			
			auto gameobject = cachedPlayer.second.pGameObject;
			std::cout << "gameobject: " << gameobject << "\n";
			if (!gameobject) continue;
			UnityEngine::Object::Destroy(gameobject);
		}
	}

	void unload_assetbundles()
	{
		static bool unloadingBundles = false;
		if (!unloadingBundles) {
			unloadingBundles = true;
			auto bundles = cache::bundles();
			for (auto bundle : bundles) {
				std::cout << "unloading " << bundle.first << "    at : " << bundle.second << std::endl;
				bundle.second->Unload(true);
			}
			bundles.clear();
		}
	}

	UnityEngine::AssetBundle* load_assetbundle(std::string path)
	{
		auto bundle = cache::bundle(path);
		if (!bundle) {
			bundle = UnityEngine::AssetBundle::LoadFromFile(Il2cppLib::api::new_string<System::String*>(path));
			cache::add(bundle, path);
			std::cout << "loaded new bundle : " << path << "    at : " << bundle << std::endl;
		}
		else {
			std::cout << "fetched loaded bundle : " << path << "    at : " << bundle << std::endl;
		}
		return bundle;
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
		
		HOOK(BP_OnLand);

		void* projectile_shoot_rpc = (void*)((uintptr_t)GetModuleHandleA("GameAssembly.dll") + 0x1147120);
		hooking::New("projectile_shoot", projectile_shoot_rpc, (void*)hk__projectile_shoot);
		hooking::Enable("projectile_shoot");

		//HOOK(PVS_Receive);
		//HOOK(BP_SendVoiceData);
		//HOOK(BP_OnVoiceData);

		return true;
	}

	void unload()
	{
		std::cout << "unloading" << std::endl;
		cheat::state(cheat::status::unloading);
		while (!cheat::has_unloaded()) {
			std::cout << "wating for unload..." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			//cheat::has_unloaded(true);
		}
		hooking::Kill();
		MH_Uninitialize();
	}
}