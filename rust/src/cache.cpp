#include "cache.hpp"

#include <iostream>
#include <format>
#include <unordered_map>
#include <map>
#include <vector>

#include "cheat.hpp"

namespace cache
{
	
	std::unordered_map<std::string, debugDrawable> __debugDraws;

	UnityEngine::Camera* camera_main{};
	BasePlayer* localplayer{};

	std::map<BasePlayer*, CachedPlayer> __cachedPlayers;
	std::unordered_map<std::string, UnityEngine::AssetBundle*> assetbundles;

	std::unordered_map<std::string, uintptr_t> __arbitraryPointers;

	void _ptr(std::string key, uintptr_t value)
	{
		__arbitraryPointers[key] = value;
	}
	uintptr_t _ptr(std::string key)
	{
		return __arbitraryPointers[key];
	}

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
	void removeDraw(std::string id)
	{
		__debugDraws.erase(id);
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
	std::map<BasePlayer*, CachedPlayer>& cachedPlayers()
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

	void validatePlayerCache()
	{
		//std::cout << "validating players\n";
		auto playerList = BasePlayer::visiblePlayerList();
		auto cacheCopy = __cachedPlayers;

		for (auto player : cacheCopy ) {
			if (!playerList->Contains(player.first)) {
				cheat::revert_model_change(__cachedPlayers[player.first]);
				__cachedPlayers.erase(player.first);
				std::cout << "invalidated Cached Player\n";
			}
		}
	}
}