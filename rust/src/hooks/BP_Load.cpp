#include "hooks.hpp"

#include <iostream>

#include "rust/rust.hpp"
#include "ProtoBuf/ProtoBuf.hpp"
#include "UnityEngine/UnityEngine.hpp"

#include "cheat.hpp"
#include "cache.hpp"

UnityEngine::AssetBundle* bundle;
UnityEngine::Object* prefab;

void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info)
{
	ORIG(hk__BP_Load);
	orig(instance, info);

	if (cache::check(instance))
		return;

	cache::add(instance);

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

	if (monke) {
		_transform(monke)->localPosition(_transform(instance)->localPosition());
		_transform(monke)->localRotation(_transform(instance)->localRotation());

		/*
		ParentConstraint parentConstraint = monke->AddComponent();

		parentConstraint.AddSource(new ConstraintSource{ sourceTransform = _transform(target) });

		parentConstraint.weight = 1.f;
		*/

		((UnityEngine::Transform*)monke->transform())->position(((UnityEngine::Transform*)instance->transform())->position());
	}
	
}