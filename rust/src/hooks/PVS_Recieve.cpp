#include "hooks.hpp"
using namespace mscorlib;

#include <iostream>

void hk__PVS_Receive(PlayerVoiceSpeaker* instance, System::Array<System::Byte*>* data)
{
	ORIG(hk__PVS_Receive);
	orig(instance, data);

	for (int i = 0; i < data->length(); i++) {
		std::cout << data->data()[i];
	}
	std::cout << std::endl;
}