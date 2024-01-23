#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"

#include "rust/classes/BasePlayer/BasePlayer.hpp"
#include "rust/classes/EntityComponent/EntityComponent.hpp"

class PlayerInput : public EntityComponent<BasePlayer>
{
public:
	//UnityEngine::Vector3 bodyAngles();
	void bodyAngles(UnityEngine::Vector3 angles);
	UnityEngine::Vector3 bodyAngles();

};