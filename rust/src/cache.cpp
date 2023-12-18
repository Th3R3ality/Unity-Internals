#include "cache.hpp"

#include <iostream>

#include <unordered_map>
#include <vector>


namespace cache
{
	

	BasePlayer* localplayer{ 0 };

	std::unordered_map<BasePlayer*, CachedPlayer> __cachedPlayers;

	std::unordered_map<std::string, UnityEngine::AssetBundle*> assetbundles;
	std::unordered_map<void*, UnityEngine::GameObject*> custom_gameObjects;

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
	
	void go(BasePlayer* bp, UnityEngine::GameObject* go)
	{
		custom_gameObjects[(void*)bp] = go;
	}
	void go(void* peter, UnityEngine::GameObject* go)
	{
		custom_gameObjects[peter] = go;
	}

	UnityEngine::AssetBundle* bundle(std::string path)
	{
		return assetbundles[path];
	}

	std::unordered_map<void*, UnityEngine::GameObject*>& gameObjects()
	{
		return custom_gameObjects;
	}
	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles()
	{
		return assetbundles;
	}
	std::unordered_map<BasePlayer*, CachedPlayer>& cachedPlayers()
	{
		return __cachedPlayers;
	}

	bool check()
	{
		return localplayer ? true : false;
	}
	bool check(BasePlayer* bp)
	{
		for (auto&& cachedPlayer : __cachedPlayers) {
			if (bp == cachedPlayer.first)
				return true;
		}
		return false;
	}
	
	bool check_model(BasePlayer* bp)
	{
		if (custom_gameObjects.find((void*)bp) != custom_gameObjects.end()) {
			return true;
		}
		return false;
	}

	UnityEngine::GameObject* get_model(BasePlayer* bp)
	{
		if (custom_gameObjects.find(bp) != custom_gameObjects.end()) {
			return custom_gameObjects[bp];
		}
		return { 0 };
	}

	void local(BasePlayer* bp)
	{
		localplayer = bp;
	}
	UnityEngine::GameObject* local()
	{
		if (!localplayer)
			return nullptr;
		return localplayer->gameObject();
	}


}