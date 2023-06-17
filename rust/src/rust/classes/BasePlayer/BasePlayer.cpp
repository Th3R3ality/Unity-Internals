#include "BasePlayer.hpp"
#include "Il2cppLib.h"

System::String* BasePlayer::displayName()
{
	static auto fn = (System::String * (*)(BasePlayer*))Il2cppLib::method_from_signature("BasePlayer.get_displayName()");
	return fn(this);
}
