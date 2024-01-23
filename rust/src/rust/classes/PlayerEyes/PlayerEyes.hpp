#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Quaternion/Quaternion.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/EntityComponent/EntityComponent.hpp"

class PlayerEyes : public EntityComponent<BasePlayer>
{
public:

	void bodyRotation(UnityEngine::Quaternion rotation);
	void bodyRotation(UnityEngine::Vector3 angles)
	{
		return bodyRotation(UnityEngine::Quaternion::Euler(angles));
	}
};