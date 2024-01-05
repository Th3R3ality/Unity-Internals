#include "cache.hpp"

#include <iostream>
#include <format>
#include <unordered_map>
#include <vector>

#include "cheat.hpp"

namespace cache
{
	
	std::unordered_map<std::string, debugDrawable> __debugDraws;

	UnityEngine::Camera* camera_main{};
	BasePlayer* localplayer{};

	std::unordered_map<BasePlayer*, CachedPlayer> __cachedPlayers;
	std::unordered_map<std::string, UnityEngine::AssetBundle*> assetbundles;


	void add(BasePlayer* bp)
	{
		__cachedPlayers[bp] = { bp, nullptr };
	}
	void add(UnityEngine::AssetBundle* bundle, std::string path)
	{
		assetbundles[path] = bundle;
	}
	
	void set(BasePlayer* bp, UnityEngine::Avatar* avatar)
	{
		__cachedPlayers[bp].pOrigAvatar = avatar;
	}
	void set(BasePlayer* bp, UnityEngine::Animator* animator)
	{
		__cachedPlayers[bp].pAnimator = animator;
	}

	void debugDraw(std::string id, debugDrawable drawCall)
	{
		__debugDraws[id] = drawCall;
	}
	std::unordered_map<std::string, debugDrawable>& debugDrawables()
	{
		return __debugDraws;
	}

	CachedPlayer& get(BasePlayer* bp)
	{
		return __cachedPlayers[bp];
	}

	UnityEngine::AssetBundle* bundle(std::string path)
	{
		return assetbundles[path];
	}

	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles()
	{
		return assetbundles;
	}
	std::unordered_map<BasePlayer*, CachedPlayer>& cachedPlayers()
	{
		return __cachedPlayers;
	}

	bool check(BasePlayer* bp)
	{
		for (auto&& cachedPlayer : __cachedPlayers) {
			if (bp == cachedPlayer.first)
				return true;
		}
		return false;
	}

	void cameraMain(UnityEngine::Camera* cam)
	{
		camera_main = cam;
	}
	UnityEngine::Camera* cameraMain()
	{
		return camera_main;
	}

	void local(BasePlayer* bp)
	{
		localplayer = bp;
	}
	BasePlayer* local()
	{
		if (!localplayer)
			return nullptr;
		return localplayer;
	}

	void validatePlayers()
	{
		auto copiedPlayers = __cachedPlayers;
		for (auto& player : copiedPlayers) {
			if (player.first->lifestate() != BaseCombatEntity::LifeState::Alive) {
				std::cout << std::format("Invalidated Player : {:x}\n", (uintptr_t)player.first);
				cheat::revert_model_change(player.second);
				__cachedPlayers.erase(player.first);
			}
		}
		//std::cout << "{ + } validated players\n";
	}
}