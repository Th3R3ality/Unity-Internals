#include "hooks.hpp"

#include <iostream>

#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/MeshRenderer/MeshRenderer.hpp"
#include "UnityEngine/SkinnedMeshRenderer/SkinnedMeshRenderer.hpp"
#include "UnityEngine/Component/Component.hpp"
#include "UnityEngine/Animator/Animator.hpp"

#include "mscorlib/System/String/String.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"



#include "cheat.hpp"
#include "cache.hpp"

UnityEngine::AssetBundle* bundle{ 0 };
UnityEngine::Object* prefab{ 0 };
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

	std::cout << "bp_load hook!" << std::endl;
	
	auto model = instance->model();
	if (!model) {
		std::cout << "\n--------------------------------\n            no model           \n--------------------------------\n\n";
		return;
	}

	auto pModel = model->gameObject();
	std::cout << "got pModel Go\n";

	if (cache::check(instance)) {
		return;
	}
	cache::add(instance);
	std::cout << "new player found!\n";



	std::cout << std::hex << std::showbase << "instance* " << instance 
		<< std::dec << std::noshowbase << std::endl;

	auto go = instance->gameObject();
	auto name = go->name();
	std::wcout << name << std::endl;
	
	std::wcout << instance->displayName() << std::endl;

	bool lp = false;
	if (name->equals(L"LocalPlayer")) {
		cache::local(instance);
		std::cout << "set localplayer!" << std::endl;
		lp = true;
		//return;
	}
	
	std::wcout << "--- MODEL : " << pModel->name() << " ---\n";
	for (int i = 0; i < pModel->transform()->childCount(); i++) {
		std::wcout << " - " << pModel->transform()->GetChild(i)->name() << "\n";
	} std::wcout << std::endl;
	
	static auto bundle = cheat::load_assetbundle("C:\\Users\\reality\\Desktop\\monke.bundle");
	if (!prefab) {
		prefab = bundle->LoadAsset("assets/rust monke.prefab", UnityEngine::GameObject());
		std::cout << "loaded asset : " << prefab << std::endl;
	} else {
		std::cout << "bundle already loaded\n";
	}
	
	UnityEngine::GameObject* monkeModel = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
	std::cout << "instantiated monkeModel\n";
	

	auto renderers = (mscorlib::System::Array<UnityEngine::Renderer* > *)pModel->GetComponentsInChildren(UnityEngine::Renderer());
	std::cout << "got old renderers\n";

	for (int idx = 0; idx < renderers->length(); idx++) {
		std::wcout << "disabling renderer: " << renderers->data()[idx]->name() << " idx: " << idx << std::endl;
		renderers->data()[idx]->enabled(false);
	}
	monkeModel->transform()->SetParent(pModel->transform());
	std::cout << "set parent\n";

	//newGo.transform.localPosition = Vector3.zero;
	//newGo.transform.localRotation = Quaternion.identity;
	//newGo.transform.localScale = Vector3.one;

	auto newSkinnedMeshRenderer = (UnityEngine::SkinnedMeshRenderer*)(monkeModel->transform()->GetComponentsInChildren(UnityEngine::SkinnedMeshRenderer())->data()[0]);
	std::cout << "got newSkinnedMeshRenderer\n";
	auto newArmature = newSkinnedMeshRenderer->rootBone()->transform()->parent();
	std::cout << "got newArmature\n";
	newArmature->SetParent(pModel->transform());
	std::cout << "set newArmature parent\n";
	//newArmature.transform.localScale = oldArmature.transform.localScale;
	//newArmature.transform.localRotation = oldArmature.transform.localRotation;
	//newArmature.transform.localPosition = oldArmature.transform.localPosition;

	

	auto oldAnimator = (UnityEngine::Animator*)pModel->transform()->GetComponent(UnityEngine::Animator());
	std::cout << "got oldAnimator\n";
	auto newAnimator = (UnityEngine::Animator*)monkeModel->transform()->GetComponent(UnityEngine::Animator());
	std::cout << "got newAnimator\n";

	auto avatar = newAnimator->avatar();
	std::cout << "got newAnimator avatar\n";
	oldAnimator->avatar(avatar);
	std::cout << "set oldAnimator avatar -> newAnimator avatar\n";

	std::cout << "swapped avatar\n";
	std::cout << "\ndone..." << std::endl;
}