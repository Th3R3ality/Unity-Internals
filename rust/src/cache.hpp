#pragma once
#include <string>
#include <unordered_map>

#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Avatar/Avatar.hpp"
#include "UnityEngine/Animator/Animator.hpp"
#include "UnityEngine/Transform/Transform.hpp"
#include "UnityEngine/Camera/Camera.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"


namespace cache
{
	class CachedPlayer {
	public:
		BasePlayer* pBasePlayer = nullptr;
		UnityEngine::Avatar* pOrigAvatar = nullptr;
		UnityEngine::Animator* pAnimator = nullptr;
		UnityEngine::GameObject* pGameObject = nullptr;
		UnityEngine::Transform* lFoot = nullptr;
		UnityEngine::Transform* rFoot = nullptr;
	};

	enum item_type
	{
		all = 0xff,
		players = 1 << 0,
		constraints = 1 << 1,
		CHANGE_THIS_local = 1 << 2
	};

	void add(BasePlayer* bp);
	void add(UnityEngine::AssetBundle* bundle, std::string path);
	
	void set(BasePlayer* bp, UnityEngine::Avatar* avatar);
	void set(BasePlayer* bp, UnityEngine::Animator* animator);

	CachedPlayer& get(BasePlayer* bp);


	bool check(BasePlayer* bp);


	UnityEngine::AssetBundle* bundle(std::string path);
	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles();
	std::unordered_map<BasePlayer*, CachedPlayer>& cachedPlayers();


	UnityEngine::Camera* cameraMain();
	void cameraMain(UnityEngine::Camera* cam);

	void local(BasePlayer* bp);
	BasePlayer* local();
}