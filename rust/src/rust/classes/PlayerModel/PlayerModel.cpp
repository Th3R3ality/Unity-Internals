#include "rust/classes/PlayerModel/PlayerModel.hpp"

#include <iostream>

#include "Il2cppLib.h"

UnityEngine::Transform* PlayerModel::leftFootBone()
{
	//static auto fn = (UnityEngine::Transform*(*)(PlayerModel*))Il2cppLib::method_from_signature("PlayerModel.get_leftFootBone");
	//if (fn)
	//	return fn(this);
	//std::cout << "no il2cpp get_leftFootBone\n";
	//fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::resolve_icall("PlayerModel::get_leftFootBone");
	//if (fn)
	//	return fn(this);
	//std::cout << "no icall get_leftFootBone\n";

	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.leftFootBone");
	if (offset)
		return *(UnityEngine::Transform**)((std::uintptr_t)this + offset);

	std::cout << "no fucking offset PlayerModel.leftFootBone\n";

	return nullptr;
}

UnityEngine::Transform* PlayerModel::rightFootBone()
{
	//static auto fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::method_from_signature("PlayerModel.get_rightFootBone");
	//if (fn)
	//	return fn(this);
	//std::cout << "no il2cpp get_leftFootBone\n";
	//fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::resolve_icall("PlayerModel::get_rightFootBone");
	//if (fn)
	//	return fn(this);
	//std::cout << "no icall get_leftFootBone\n";

	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.rightFootBone");
	if (offset)
		return *(UnityEngine::Transform**)((std::uintptr_t)this + offset);

	std::cout << "no fucking offset PlayerModel.rightFootBone\n";

	return nullptr;
}

void PlayerModel::leftFootBone(UnityEngine::Transform* transform)
{
	//static auto fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::method_from_signature("PlayerModel.set_leftFootBone");
	//if (fn)
	//	return fn(this, transform);
	//std::cout << "no il2cpp get_leftFootBone\n";
	//fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::resolve_icall("PlayerModel::set_leftFootBone");
	//if (fn)
	//	return fn(this, transform);
	//std::cout << "no icall get_leftFootBone\n";

	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.leftFootBone");
	if (offset) {
		*(UnityEngine::Transform**)((std::uintptr_t)this + offset) = transform;
		return;
	}

	std::cout << "no fucking offset PlayerModel.leftFootBone\n";

	return;
}

void PlayerModel::rightFootBone(UnityEngine::Transform* transform)
{
	//static auto fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::method_from_signature("PlayerModel.set_rightFootBone");
	//if (fn)
	//	return fn(this, transform);
	//std::cout << "no il2cpp get_leftFootBone\n";
	//fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::resolve_icall("PlayerModel::set_rightFootBone");
	//if (fn)
	//	return fn(this, transform);
	//std::cout << "no icall get_leftFootBone\n";

	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.rightFootBone");
	if (offset) {
		*(UnityEngine::Transform**)((std::uintptr_t)this + offset) = transform;
		return;
	}

	std::cout << "no fucking offset PlayerModel.rightFootBone\n";

	return;
}


UnityEngine::Transform* PlayerModel::leftFootIK(int val)
{
	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.leftFootIK");
	//fixxxx static field ???!?!??
}

UnityEngine::Transform* PlayerModel::rightFootIK(int val)
{
	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.rightFootIK");
	//fixxxx static field ???!?!??
}