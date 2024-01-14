#pragma once
#include <string>
#include <unordered_map>
#include <map>

#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Avatar/Avatar.hpp"
#include "UnityEngine/Animator/Animator.hpp"
#include "UnityEngine/Transform/Transform.hpp"
#include "UnityEngine/Camera/Camera.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"

#include "Lapis/engine/LapisTypes.h"

namespace cache
{
	struct debugDrawable
	{
	public:
		Lapis::Shape shape;
		Lapis::Transform transform;
		Lapis::Color color;
		Lapis::Vec3 from;
		Lapis::Vec3 to;
	};

	struct debugCube : public debugDrawable
	{
	public:
		debugCube(Lapis::Transform t, Lapis::Color c)
		{
			shape = Lapis::Shape::Cube;
			transform = t;
			color = c;
		}
	};

	struct debugIcosahedron : public debugDrawable
	{
		debugIcosahedron(Lapis::Transform t, Lapis::Color c)
		{
			shape = Lapis::Shape::Icosahedron;
			transform = t;
			color = c;
		}
	};

	struct debugLine3d : public debugDrawable
	{
		debugLine3d(Lapis::Vec3 f, Lapis::Vec3 t, Lapis::Color c)
		{
			shape = Lapis::Shape::Line3d;
			from = f;
			to = t;
			color = c;
		}
	};

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
	void _ptr(std::string key, uintptr_t value);
	uintptr_t _ptr(std::string key);

	template<typename T>
	void ptr(std::string key, T value)
	{
		_ptr(key, (uintptr_t)value);
	}
	template<typename T>
	T ptr(std::string key)
	{
		return (T)_ptr(key);
	}


	void add(BasePlayer* bp);
	void add(UnityEngine::AssetBundle* bundle, std::string path);
	
	void set(BasePlayer* bp, UnityEngine::Avatar* avatar);
	void set(BasePlayer* bp, UnityEngine::Animator* animator);

	void removeDraw(std::string id);
	void debugDraw(std::string id, debugDrawable drawCall);
	std::unordered_map<std::string, debugDrawable> debugDrawables();


	CachedPlayer& get(BasePlayer* bp);


	bool check(BasePlayer* bp);

	UnityEngine::AssetBundle* bundle(std::string path);
	std::unordered_map<std::string, UnityEngine::AssetBundle*>& bundles();
	std::map<BasePlayer*, CachedPlayer>& cachedPlayers();


	UnityEngine::Camera* cameraMain();
	void cameraMain(UnityEngine::Camera* cam);

	void local(BasePlayer* bp);
	BasePlayer* local();

	void validatePlayerCache();
}