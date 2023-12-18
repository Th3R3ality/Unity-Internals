#pragma once
#include "rust/classes/FacepunchBehaviour/FacepunchBehaviour.hpp"
#include "UnityEngine/Transform/Transform.hpp"

class PlayerModel : public FacepunchBehaviour
{
public:
	type_overload("PlayerModel");

	UnityEngine::Transform* leftFootBone();
	UnityEngine::Transform* rightFootBone();

	void leftFootBone(UnityEngine::Transform* transform);
	void rightFootBone(UnityEngine::Transform* transform);
};