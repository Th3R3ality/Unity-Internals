#pragma once
#include <string>
#include <unordered_map>

#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/AssetBundle/AssetBundle.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"

#include "constraints.hpp"

namespace cache
{
	enum item_type
	{
		all = 0xff,
		players = 1 << 0,
		constraints = 1 << 1,
		CHANGE_THIS_local = 1 << 2
	};

	void add(BasePlayer* bp);
	void add(UnityEngine::AssetBundle* bundle, std::string path);
	
	void go(BasePlayer* bp, UnityEngine::GameObject* go);
	void go(void*, UnityEngine::GameObject* go);

	bool check();
	bool check(BasePlayer* bp);
	bool check_model(BasePlayer* bp);

	void add_constraint(unsigned int type, UnityEngine::GameObject* src, UnityEngine::GameObject* dst, bool startEnabled = false);
	std::vector<constraint>& get_constraints();

	UnityEngine::AssetBundle* bundle(std::string path);
	std::unordered_map<void*, UnityEngine::GameObject*>& gameObjects();
	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles();

	UnityEngine::GameObject* get_model(BasePlayer* bp);

	void local(BasePlayer* bp);
	UnityEngine::GameObject* local();
}