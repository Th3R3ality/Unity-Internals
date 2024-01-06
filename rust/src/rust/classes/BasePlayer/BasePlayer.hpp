#pragma once
#include "rust/classes/BaseCombatEntity/BaseCombatEntity.hpp"
#include "rust/classes/PlayerModel/PlayerModel.hpp"

#include "mscorlib/System/String/String.hpp"
#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Byte/Byte.hpp"

using namespace mscorlib;

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

	static BufferList<BasePlayer*>* visiblePlayerList();

};