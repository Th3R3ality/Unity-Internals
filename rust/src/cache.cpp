#include "cache.hpp"

#include <iostream>

#include <unordered_map>
#include <vector>

namespace cache
{


	BasePlayer* localplayer{ 0 };
	
	std::vector<BasePlayer*> __players{ 0 };
	std::vector<constraint> __constraints;

	std::unordered_map<std::string, UnityEngine::AssetBundle*> assetbundles;
	std::unordered_map<void*, UnityEngine::GameObject*> custom_gameObjects;

	void add(BasePlayer* bp)
	{
		__players.push_back(bp);
	}
	void add(UnityEngine::AssetBundle* bundle, std::string path)
	{
		assetbundles[path] = bundle;
	}
	
	void go(BasePlayer* bp, UnityEngine::GameObject* go)
	{
		custom_gameObjects[(void*)bp] = go;
	}
	void go(void* peter, UnityEngine::GameObject* go)
	{
		custom_gameObjects[peter] = go;
	}

	void cache::add_constraint(unsigned int type, UnityEngine::GameObject* src, UnityEngine::GameObject* dst, bool startEnabled)
	{
		std::cout << "added constraint to: " << dst->name() << " -> " << src->name() << std::endl;
		__constraints.push_back(constraint(type, src, dst, startEnabled));
	}
	std::vector<constraint>& cache::get_constraints()
	{
		return __constraints;
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

	bool check()
	{
		return localplayer ? true : false;
	}
	bool check(BasePlayer* bp)
	{
		for (auto&& p : __players) {
			if (bp == p)
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