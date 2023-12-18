#include "rust/classes/PlayerModel/PlayerModel.hpp"

#include <iostream>

#include "Il2cppLib.h"

UnityEngine::Transform* PlayerModel::leftFootBone()
{
	static auto fn = (UnityEngine::Transform*(*)(PlayerModel*))Il2cppLib::method_from_signature("PlayerModel.get_leftFootBone");
	if (fn)
		return fn(this);
	std::cout << "no il2cpp get_leftFootBone\n";
	fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::resolve_icall("");
	if (fn)
		return fn(this);
	std::cout << "no icall get_leftFootBone\n";
	return nullptr;
}

UnityEngine::Transform* PlayerModel::rightFootBone()
{
	static auto fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::method_from_signature("PlayerModel.get_rightFootBone");
	if (fn)
		return fn(this);
	std::cout << "no il2cpp get_leftFootBone\n";
	fn = (UnityEngine::Transform * (*)(PlayerModel*))Il2cppLib::resolve_icall("");
	if (fn)
		return fn(this);
	std::cout << "no icall get_leftFootBone\n";
	return nullptr;
}

void PlayerModel::leftFootBone(UnityEngine::Transform* transform)
{
	static auto fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::method_from_signature("PlayerModel.set_leftFootBone");
	if (fn)
		return fn(this, transform);
	std::cout << "no il2cpp get_leftFootBone\n";
	fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::resolve_icall("");
	if (fn)
		return fn(this, transform);
	std::cout << "no icall get_leftFootBone\n";
	return;
}

void PlayerModel::rightFootBone(UnityEngine::Transform* transform)
{
	static auto fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::method_from_signature("PlayerModel.set_rightFootBone");
	if (fn)
		return fn(this, transform);
	std::cout << "no il2cpp get_leftFootBone\n";
	fn = (void(*)(PlayerModel*, UnityEngine::Transform*))Il2cppLib::resolve_icall("");
	if (fn)
		return fn(this, transform);
	std::cout << "no icall get_leftFootBone\n";
	return;
}
