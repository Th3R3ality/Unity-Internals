#include "hooks.hpp"

#include <iostream>

#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/MeshRenderer/MeshRenderer.hpp"
#include "UnityEngine/SkinnedMeshRenderer/SkinnedMeshRenderer.hpp"
#include "UnityEngine/Component/Component.hpp"

#include "mscorlib/System/String/String.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"



#include "cheat.hpp"
#include "cache.hpp"

UnityEngine::AssetBundle* bundle;
UnityEngine::Object* prefab;

void LABaAC(UnityEngine::Transform* destination, UnityEngine::Transform* source)
{
	for (int i = 0; i < destination->childCount(); i++) {
		auto dst_bone = destination->GetChild(i);
		auto src_bone = UnityEngine::Transform::RecursiveFindChild(source, dst_bone->name());
		if (src_bone != nullptr) {
			cache::add_constraint(constraint_type::rotation, src_bone->gameObject(), dst_bone->gameObject(), true);
		}
		LABaAC(destination->GetChild(i), source);
	}
}

void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	ORIG(hk__BP_Load);
	orig(instance, info);

	if (cache::check(instance))
		return;

	cache::add(instance);

	std::cout << "bp_load hook!" << std::endl;

	auto go = instance->gameObject();
	if (!go) {
		std::cout << "fail - go" << std::endl;
		return;
	}

	auto name = go->name();
	if (!name) {
		std::cout << "fail - name" << std::endl;
		return;
	}
	std::wcout << name << std::endl;

	if (name->equals(L"LocalPlayer")) {
		cache::local(instance);
		std::cout << "set localplayer!" << std::endl;
	}

	
	auto bundle = cheat::load_assetbundle("C:\\Users\\reality\\Desktop\\monke.bundle");

	if (!prefab) {
		prefab = bundle->LoadAsset("assets/monke.prefab", UnityEngine::GameObject());
		std::cout << "loaded asset : " << prefab << std::endl;
	}

	UnityEngine::GameObject* monke = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
	cache::go(instance, monke);
	std::cout << "monke : " << monke << "    added to    baseplayer : " << instance << std::endl;
    
	auto player = instance->gameObject();
	
	cache::add_constraint(constraint_type::position, player, monke, true);

	UnityEngine::Transform* player_pelvis = UnityEngine::Transform::RecursiveFindChild(player->transform(), L"Pelvis");
	UnityEngine::Transform* model_pelvis = UnityEngine::Transform::RecursiveFindChild(monke->transform(), L"Pelvis");
	
	{
		auto mesh_renderers = player->GetComponentsInChildren(UnityEngine::MeshRenderer());
		for (int idx = 0; idx < mesh_renderers->length(); idx++) {
			mesh_renderers->data()[idx]->gameObject()->SetActive(false);
		}
	}
	{
		auto skinnedmesh_renderers = player->GetComponentsInChildren(UnityEngine::SkinnedMeshRenderer());
		for (int idx = 0; idx < skinnedmesh_renderers->length(); idx++) {
			skinnedmesh_renderers->data()[idx]->gameObject()->SetActive(false);
		}
	}
	
	LABaAC(model_pelvis, player_pelvis);
}