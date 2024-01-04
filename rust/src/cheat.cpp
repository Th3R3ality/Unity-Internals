#include "cheat.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include <d3d11.h>

#include "minhook/include/MinHook.h"

#include "hooks/hooking2.hpp"
#include "hooks/hooks.hpp"

#include "UnityEngine/UnityEngine.hpp"

#include "cache.hpp"
#include "global.h"


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

	uintptr_t swapchainVMT;
	bool init_hooks()
	{
		//directx hooks
		{
			// Create Dummy SwapChain to get VTable
			IDXGISwapChain* pSwapchain = nullptr;
			{
				DXGI_SWAP_CHAIN_DESC scd{};
				scd.BufferCount = 1;                                    // one back buffer
				scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
				scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
				scd.OutputWindow = GetForegroundWindow();                                // the window to be used
				scd.SampleDesc.Count = 4;                               // how many multisamples
				scd.Windowed = TRUE;                                    // windowed/full-screen mode
				scd.BufferDesc.Width = 800;
				scd.BufferDesc.Height = 600;
				scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				HRESULT hr = D3D11CreateDeviceAndSwapChain(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					0,
					nullptr,
					0,
					D3D11_SDK_VERSION,
					&scd,
					&pSwapchain,
					nullptr,
					nullptr,
					nullptr);

				std::cout << "D3D11CreateDeviceAndSwapChain : " << std::system_category().message(hr) << "\n";
				std::cout << "pSwapchain : " << pSwapchain << "\n";
			}

			if (pSwapchain) {
				swapchainVMT = *(uintptr_t*)pSwapchain;
				safe_release(pSwapchain);

				origPresent = VMTEntryHook(swapchainVMT, (size_t)IDXGISwapChainVMT::Present, (uintptr_t)hkPresent);
				origResizeBuffers = VMTEntryHook(swapchainVMT, (size_t)IDXGISwapChainVMT::ResizeBuffers, (uintptr_t)hkResizeBuffers);
			}
		}


		MH_Initialize();

		//hooking::New("pu_update", Il2cppLib::method_from_signature(hk__FP_PU_Update_sig), (void*)hk__FP_PU_Update);
		//hooking::Enable("pu_update");

		HOOK(FP_PU_Update);
		HOOK(BP_Load);
		HOOK(PWM_HandleJumping);
		HOOK(HE_AddPunch);
		
		HOOK(BP_OnLand);


		std::cout << " GameAssembly.dll : " << (uintptr_t)GetModuleHandleA("GameAssembly.dll") << "\n";
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

		ejecting = true;
		
		std::cout << "waiting for present reset...\n";
		while (!presentReset) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		VMTEntryHook(swapchainVMT, (size_t)IDXGISwapChainVMT::Present, origPresent);
		VMTEntryHook(swapchainVMT, (size_t)IDXGISwapChainVMT::ResizeBuffers, origResizeBuffers);

		std::cout << "wating for unload..." << std::endl;
		while (!cheat::has_unloaded()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			//cheat::has_unloaded(true);
		}
		hooking::Kill();
		MH_Uninitialize();

		std::cout << "[ + ] Unloaded\n";
	}
}