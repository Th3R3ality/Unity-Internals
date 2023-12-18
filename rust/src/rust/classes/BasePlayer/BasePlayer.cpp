#include "BasePlayer.hpp"
#include "Il2cppLib.h"
#include <iostream>
System::String* BasePlayer::displayName()
{
	static auto fn = (System::String * (*)(BasePlayer*))Il2cppLib::method_from_signature("BasePlayer.get_displayName()");
	return fn(this);
}

void BasePlayer::SendVoiceData(System::Array<System::Byte*>* data, int len)
{
	static auto fn = (void(*)(BasePlayer*, System::Array<System::Byte*>*, int))Il2cppLib::method_from_signature("BasePlayer.SendVoiceData(System.Byte[], int)");
	return fn(this, data, len);
}

PlayerModel* BasePlayer::playerModel()
{
	static auto fn = (PlayerModel*(*)(BasePlayer*))Il2cppLib::method_from_signature("BasePlayer.get_playerModel()", true);
	if (fn)
		return fn(this);
	std::cout << "no fucking il2cpp playerModel\n";

	fn = (PlayerModel * (*)(BasePlayer*))Il2cppLib::resolve_icall("BasePlayer::get_playerModel()");
	if (fn)
		return fn(this);
	std::cout << "no fucking il2cpp playerModel\n";

	return nullptr;
}

