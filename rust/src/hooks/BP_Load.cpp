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

UnityEngine::AssetBundle* bundle{ 0 };
UnityEngine::Object* prefab{ 0 };
UnityEngine::GameObject* monke_root{ 0 };
//Loop All Bones
//and
//Add Constraints
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
	
	auto pModel = instance->model();
	if (!pModel) {
		std::cout << "\n--------------------------------\n            no model           \n--------------------------------\n\n";
		return;
	}

	if (cache::check(instance))
		return;

	cache::add(instance);

	std::cout << "bp_load hook!" << std::endl;

	std::cout << std::hex << std::showbase << "instance* " << instance 
		<< std::dec << std::noshowbase << std::endl;

	auto go = instance->gameObject();
	auto name = go->name();
	std::wcout << go->name() << std::endl;
	
	std::wcout << instance->displayName() << std::endl;

	bool lp = false;
	if (name->equals(L"LocalPlayer")) {
		cache::local(instance);
		std::cout << "set localplayer!" << std::endl;
		lp = true;
		return;
	}

	/*
	
	UnityEngine::GameObject* player_model_root = pModel->transform()->root()->gameObject();
	std::wcout << "--- MODEL : " << player_model_root->name() << " ---\n";
	for (int i = 0; i < player_model_root->transform()->childCount(); i++) {
		std::wcout << " - " << player_model_root->transform()->GetChild(i)->name() << "\n";
	} std::wcout << std::endl;
	
	static auto bundle = cheat::load_assetbundle("C:\\Users\\reality\\Desktop\\monke.bundle");
	if (!prefab) {
		prefab = bundle->LoadAsset("assets/monke.prefab", UnityEngine::GameObject());
		std::cout << "loaded asset : " << prefab << std::endl;
	}

	//if (monke_root)
	//	return;

	

	monke_root = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
	//UnityEngine::GameObject* player_model_root = instance->model()->transform()->root()->gameObject();
	//already defined ealier l:70

	cache::add_constraint(constraint_type::position | constraint_type::rotation, player_model_root, monke_root, true);
	std::cout << "adding constraint to\n\tmonke_root : " << monke_root->name() << "\nconstrained to\n\tbaseplayer : " << instance->name() << std::endl;
  
	std::cout << "searching for \"Pelvis\" on player: " << player_model_root->name() << std::endl;
	UnityEngine::Transform* player_pelvis = UnityEngine::Transform::RecursiveFindChild(player_model_root->transform(), L"pelvis");

	std::cout << "searching for \"Pelvis\" on player: monke_root" << std::endl;
	UnityEngine::Transform* model_pelvis = UnityEngine::Transform::RecursiveFindChild(monke_root->transform(), L"pelvis");
	
	std::cout << "\n";
	if (player_pelvis && model_pelvis)
	{
		auto mesh_renderers = player_model_root->GetComponentsInChildren(UnityEngine::MeshRenderer());
		for (int idx = 0; idx < mesh_renderers->length(); idx++) {
			std::wcout << "disabling mesh_renderer: " << mesh_renderers->data()[idx]->name() << std::endl;
			mesh_renderers->data()[idx]->gameObject()->SetActive(false);
		}
		std::cout << "\n";
		auto skinnedmesh_renderers = player_model_root->GetComponentsInChildren(UnityEngine::SkinnedMeshRenderer());
		for (int idx = 0; idx < skinnedmesh_renderers->length(); idx++) {
			std::wcout << "disabling mesh_renderer: " << skinnedmesh_renderers->data()[idx]->name() << std::endl;
			skinnedmesh_renderers->data()[idx]->gameObject()->SetActive(false);
		}
		
		LABaAC(model_pelvis, player_pelvis);
	}

	*/
	
	std::cout << "\ndone..." << std::endl;
}