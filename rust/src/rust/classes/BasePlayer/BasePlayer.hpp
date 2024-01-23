#pragma once
#include "mscorlib/System/String/String.hpp"
#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Byte/Byte.hpp"
using namespace mscorlib;

#include "rust/classes/BaseCombatEntity/BaseCombatEntity.hpp"
#include "rust/classes/PlayerModel/PlayerModel.hpp"
#include "rust/classes/BaseMovement/BaseMovement.hpp"

class PlayerEyes;
class PlayerInput;

class BasePlayer : public BaseCombatEntity
{
public:

	enum CameraMode
	{
		FirstPerson,
		ThirdPerson,
		Eyes,
		FirstPersonWithArms,
		DeathCamClassic,
		Last = 3
	};
	System::String* displayName();

	void SendVoiceData(System::Array<System::Byte*>* data, int len);

	PlayerModel* playerModel();
	BaseMovement* movement();
	PlayerEyes* eyes();
	PlayerInput* input();

	static BufferList<BasePlayer*>* visiblePlayerList();

};