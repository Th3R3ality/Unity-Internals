#include "hk_load.hpp"
#include "hooks/hooking.hpp"

#include <iostream>

#include "rust/rust.hpp"
#include "ProtoBuf/ProtoBuf.hpp"
#include "UnityEngine/UnityEngine.hpp"

//#include "caching.hpp"
//#include <vector>

void hk__BasePlayer_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	static auto orig = hooking::original(hk__BasePlayer_Load_sig);
	if (!orig) return;
	((decltype(&hk__BasePlayer_Load))*orig)(instance, info);

	//UnityEngine::GameObject* go = instance->gameObject();


	std::cout << "bp_load hook!" << std::endl;
	/*
	auto iter = std::find(caching::players::begin(), caching::players::end(), instance);
	if (iter == caching::players::end()) {
		std::cout << "cached new player" << std::endl;
		caching::players::push_back(instance);
	}
	*/
}