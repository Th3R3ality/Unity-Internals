#include "hooks.hpp"

#include <iostream>
#include <Windows.h>
#include <cheat.hpp>

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{
	static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
			std::cout << orig << " : orig much???" << std::endl;
		return;
	}

	orig(instance);

	if (counter % 5000 == 0)
		std::cout << "pu_update hook!" << std::endl;


	if (GetAsyncKeyState(0x4C) & 0x0001) {

		auto bundle = cheat::load_assetbundle("C:\\Users\\reality\\Desktop\\monke.bundle");
		std::cout << bundle << " : bundle\n";

		//static auto prefab = bundle->LoadAsset("assets/monke.prefab", UnityEngine::GameObject());
		//std::cout << prefab << " : prefab\n";

		//UnityEngine::GameObject* monke = (UnityEngine::GameObject*)UnityEngine::Object::Instantiate(prefab);
		//std::cout << monke << " : monke" << std::endl;

		//if (monke)
		//	((UnityEngine::Transform*)monke->transform())->position(((UnityEngine::Transform*)cache::local()->transform())->position());



		//UnityEngine::GameObject* sphere = UnityEngine::GameObject::CreatePrimitive(UnityEngine::PrimitiveType::Sphere);
		//((UnityEngine::Transform*)sphere->transform())->position(((UnityEngine::Transform*)cache::local()->transform())->position());

	}
	
	if (cheat::state() == cheat::status::unloading) {
		//cheat::unload_gameObjects();
		//cheat::unload_assetbundles();
		cheat::has_unloaded(true);
	}
	
}