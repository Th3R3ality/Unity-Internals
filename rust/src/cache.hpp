#pragma once
#include <string>
#include <unordered_map>

#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/AssetBundle/AssetBundle.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"

namespace cache
{
	void add(BasePlayer* bp);
	void add(UnityEngine::AssetBundle* bundle, std::string path);
	
	void go(BasePlayer* bp, UnityEngine::GameObject* go);
	void go(void*, UnityEngine::GameObject* go);

	bool check();
	bool check(BasePlayer* bp);
	bool check_model(BasePlayer* bp);

	UnityEngine::AssetBundle* bundle(std::string path);
	std::unordered_map<void*, UnityEngine::GameObject*>& gameObjects();
	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles();

	UnityEngine::GameObject* get_model(BasePlayer* bp);

	void local(BasePlayer* bp);
	UnityEngine::GameObject* local();
}