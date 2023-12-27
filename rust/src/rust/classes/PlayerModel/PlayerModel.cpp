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
	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.rightFootBone");
	if (offset)
		return *(UnityEngine::Transform**)((std::uintptr_t)this + offset);

	std::cout << "no fucking offset PlayerModel.rightFootBone\n";

	return nullptr;
}

void PlayerModel::leftFootBone(UnityEngine::Transform* transform)
{
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
	static int offset = Il2cppLib::field_offset_from_signature("PlayerModel.rightFootBone");
	if (offset) {
		*(UnityEngine::Transform**)((std::uintptr_t)this + offset) = transform;
		return;
	}

	std::cout << "no fucking offset PlayerModel.rightFootBone\n";

	return;
}

void PlayerModel::leftFootIK(int val)
{
	auto ptr = (int*)Il2cppLib::static_field_from_signature("PlayerModel.leftFootIK");
	if (ptr)
		*ptr = val;
}

void PlayerModel::rightFootIK(int val)
{
	auto ptr = (int*)Il2cppLib::field_offset_from_signature("PlayerModel.rightFootIK");
	if (ptr)
		*ptr = val;
}