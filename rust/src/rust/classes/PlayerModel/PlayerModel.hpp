#pragma once
#include "rust/classes/FacepunchBehaviour/FacepunchBehaviour.hpp"
#include "UnityEngine/Transform/Transform.hpp"

class PlayerModel : public FacepunchBehaviour
{
public:
	type_overload("PlayerModel");

	UnityEngine::Transform* leftFootBone();
	void leftFootBone(UnityEngine::Transform* transform);
	UnityEngine::Transform* rightFootBone();
	void rightFootBone(UnityEngine::Transform* transform);

	UnityEngine::Transform* leftFootIK(int val);
	UnityEngine::Transform* rightFootIK(int val);
};