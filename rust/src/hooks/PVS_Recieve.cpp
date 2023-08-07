#include "hooks.hpp"
using namespace mscorlib;

#include "rust/classes/EntityComponent/EntityComponent.hpp"

#include <iostream>

void hk__PVS_Receive(PlayerVoiceSpeaker* instance, System::Array<System::Byte*>* data)
{
	ORIG(hk__PVS_Receive);
	orig(instance, data);
	std::cout << std::hex;

	std::cout << (uint64_t)(data->data()[0]) << " ";

	for (int i = sizeof(uint64_t); i < data->length(); i++) {
		std::cout << (uintptr_t)((uint8_t*)data->data())[i];
	}

	std::cout << std::dec << std::endl;

	//crash
	//((BasePlayer*)instance->GetBaseEntity())->SendVoiceData(data, data->length());
}