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
	//static auto fn = (PlayerModel*(*)(BasePlayer*))Il2cppLib::method_from_signature("BasePlayer.get_playerModel()", true);
	//if (fn)
	//	return fn(this);
	//std::cout << "no fucking il2cpp get_playerModel\n";

	//fn = (PlayerModel * (*)(BasePlayer*))Il2cppLib::resolve_icall("BasePlayer::get_playerModel()");
	//if (fn)
	//	return fn(this);
	//std::cout << "no fucking icall get_playerModel\n";

	static auto offset = Il2cppLib::field_offset_from_signature("BasePlayer.playerModel");
	if (offset)
		return *(PlayerModel**)((std::uintptr_t)this + offset);

	std::cout << "no fucking offset playerModel\n";
	return nullptr;
}

BufferList<BasePlayer*>* BasePlayer::visiblePlayerList()
{
	static auto fn = (BufferList<BasePlayer*>*(*)())Il2cppLib::method_from_signature("BasePlayer.get_VisiblePlayerList()");
	if (fn)
		return fn();
	return nullptr;
}


