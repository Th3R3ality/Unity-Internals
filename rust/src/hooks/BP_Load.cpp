#include "hooks.hpp"

#include <iostream>

#include "rust/rust.hpp"
#include "ProtoBuf/ProtoBuf.hpp"
#include "UnityEngine/UnityEngine.hpp"

//#include "caching.hpp"
//#include <vector>

void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	ORIG(hk__BP_Load);

	if (!orig) return;

	orig(instance, info);

	//UnityEngine::GameObject* go = instance->gameObject();

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

	std::wcout << go->name() << std::endl;

	/*
	auto iter = std::find(caching::players::begin(), caching::players::end(), instance);
	if (iter == caching::players::end()) {
		std::cout << "cached new player" << std::endl;
		caching::players::push_back(instance);
	}
	*/
}