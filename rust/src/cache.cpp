#include "cache.hpp"

#include <iostream>

#include <unordered_map>
#include <vector>


namespace cache
{
	

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
}