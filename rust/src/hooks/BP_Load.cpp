#include "hooks.hpp"

#include <iostream>

#include "UnityEngine/AssetBundle/AssetBundle.hpp"
#include "UnityEngine/Object/Object.hpp"
#include "UnityEngine/GameObject/GameObject.hpp"
#include "UnityEngine/MeshRenderer/MeshRenderer.hpp"
#include "UnityEngine/SkinnedMeshRenderer/SkinnedMeshRenderer.hpp"
#include "UnityEngine/Component/Component.hpp"
#include "UnityEngine/Animator/Animator.hpp"
#include "UnityEngine/enums/HumanBodyBones.hpp"

#include "mscorlib/System/String/String.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"

#include "cheat.hpp"
#include "cache.hpp"

#include "Il2cppLib.h"

void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	ORIG(hk__BP_Load);
	orig(instance, info);

	//std::cout << "bp_load hook!" << std::endl;
	
	auto playerModel = instance->playerModel();
	//std::cout << "playerModel : " << playerModel << std::endl;

	if (!playerModel) {
		std::cout << "\n--------------------------------\n|        no playerModel        |\n--------------------------------\n\n";
		return;
	}

	if (cache::check(instance)) {
		return;
	}
	cache::add(instance);
	cache::CachedPlayer& cachedPlayer = cache::get(instance);
	std::cout << "new player found!\n";

	auto pModel = playerModel->gameObject();
	std::cout << "got pModel Go\n";


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

		if (!cache::cameraMain()) {

			if (auto cam = UnityEngine::Camera::main()) {

				cache::cameraMain(cam);
			} else {
				std::cout << "COULDNT GET MAIN CAM!!!!!!!!!!!!!!!\n";
			}
		}
	}
	
	return; // dont do model changer

	std::wcout << "--- MODEL : " << pModel->name() << " ---\n";
	for (int i = 0; i < pModel->transform()->childCount(); i++) {
		std::wcout << " - " << pModel->transform()->GetChild(i)->name() << "\n";
	} std::wcout << std::endl;
	
	std::string modelPaths[] = {
		"C:\\Users\\reality\\Desktop\\peter.bundle",
		"C:\\Users\\reality\\Desktop\\monke.bundle",
		"C:\\Users\\reality\\Desktop\\modelchange.bundle",
		"C:\\Users\\reality\\Desktop\\gooning.bundle"
	};
	std::string prefabPaths[] = {
		"assets\\peter\\peter.prefab",
		"assets\\rust monke.prefab",
		"assets\\cxtgirl\\catgxrl_modelchange.prefab",
		"assets\\gooning.prefab"
	};

	int model = 3;

	static auto bundle = cheat::load_assetbundle(modelPaths[model]);
	static UnityEngine::Object* monkePrefab = nullptr;

	if (!monkePrefab) {
		monkePrefab = bundle->LoadAsset(prefabPaths[model], UnityEngine::GameObject());
		std::cout << "loaded monkePrefab : " << monkePrefab << std::endl;
	} else {
		std::cout << "monkePrefab already loaded\n";
	}
	
	static auto invicibleBundle = cheat::load_assetbundle("C:\\users\\reality\\Desktop\\invicible.bundle");
	static UnityEngine::Shader* invicibleShader = nullptr;
	
	if (!invicibleShader) {
		invicibleShader = (UnityEngine::Shader*)invicibleBundle->LoadAsset("assets\\invicible.shader", UnityEngine::Shader());
		std::cout << "loaded invicibleShader : " << invicibleShader << std::endl;
	}
	else {
		std::cout << "invicibleShader already loaded\n";
	}

	UnityEngine::GameObject* monkeModel = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(monkePrefab);

	auto renderers = (mscorlib::System::Array<UnityEngine::Renderer* > *)pModel->GetComponentsInChildren(UnityEngine::Renderer());

	for (int idx = 0; idx < renderers->length(); idx++) {
		auto renderer = renderers->data()[idx];
		
		bool isSkin = false;

		auto materials = renderer->materials();
		//std::cout << "material shaders:\n";
		for (int jdx = 0; jdx < materials->length(); jdx++) {
			auto material = materials->data()[jdx];
			if (!material) continue;

			auto shader = material->shader();
			std::wcout << " L " << shader->name() << std::endl;
			if (shader->name()->equals(L"Core/Skin")) {
				isSkin = true;
			}
		}
		if (isSkin) {
			std::wcout << "disabling renderer: " << renderer->name() << " idx: " << idx << std::endl;
			renderer->enabled(false);
		}
	}
	monkeModel->transform()->SetParent(pModel->transform(), false);
	cachedPlayer.pGameObject = monkeModel;
	//newGo.transform.localPosition = Vector3.zero;
	//newGo.transform.localRotation = Quaternion.identity;
	//newGo.transform.localScale = Vector3.one;

	//auto newSkinnedMeshRenderer = (UnityEngine::SkinnedMeshRenderer*)(monkeModel->transform()->GetComponentsInChildren(UnityEngine::SkinnedMeshRenderer())->data()[0]);
	//auto newArmature = newSkinnedMeshRenderer->rootBone()->transform()->parent();
	//cachedPlayer.pArmatureGo = newArmature->gameObject();
	//newArmature->SetParent(pModel->transform());
	//newArmature.transform.localScale = oldArmature.transform.localScale;
	//newArmature.transform.localRotation = oldArmature.transform.localRotation;
	//newArmature.transform.localPosition = oldArmature.transform.localPosition;

	auto oldAnimator = (UnityEngine::Animator*)pModel->transform()->GetComponent(UnityEngine::Animator());
	auto newAnimator = (UnityEngine::Animator*)monkeModel->transform()->GetComponent(UnityEngine::Animator());
	cachedPlayer.pAnimator = oldAnimator;

	
	//auto lFoot = oldAnimator->GetBoneTransform(UnityEngine::HumanBodyBones::LeftFoot);
	auto lFoot = playerModel->leftFootBone();
	if (lFoot) {
		std::cout << "lFoot found\n";
		std::cout << lFoot->name() << std::endl;
	}
	else std::cout << "no lFoot found!!!\n";

	//auto rFoot = oldAnimator->GetBoneTransform(UnityEngine::HumanBodyBones::RightFoot);
	auto rFoot = playerModel->rightFootBone();
	if (rFoot) {
		std::cout << "rFoot found\n";
		std::cout << rFoot->name() << std::endl;
		cachedPlayer.rFoot = rFoot;
		
	}
	else std::cout << "no rFoot found!!!\n";
	
	playerModel->leftFootIK(0);
	playerModel->rightFootIK(0);

	auto oldAvatar = oldAnimator->avatar();
	auto newAvatar = newAnimator->avatar();
	cachedPlayer.pOrigAvatar = oldAvatar;

	oldAnimator->avatar(newAvatar);

	std::cout << "swapped models..." << std::endl;
}