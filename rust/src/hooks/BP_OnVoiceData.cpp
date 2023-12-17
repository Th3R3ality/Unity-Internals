#include "hooks.hpp"
#include <iostream>


#include "Util/fast_crc32.h"
//#include "Util/color-console.h"

void hk__BP_OnVoiceData(BasePlayer* instance, System::Array<System::Byte*>* data)
{
	ORIG(hk__BP_OnVoiceData);
/*
	std::cout << "incoming voice data: \n";
	
	std::cout << std::dec << "steam64: " << (uint64_t)(data->data()[0]) << std::endl;
	std::cout << std::hex << "payload type: " << +((uint8_t*)data->data())[8] << std::endl;
	std::cout << std::dec << "sample rate: " << (uint16_t)((uint8_t*)data->data())[9] << std::endl;
	std::cout << std::hex << "payload type: " << +((uint8_t*)data->data())[11] << std::endl;
	std::cout << std::dec << "data length: " << (uint16_t)((uint8_t*)data->data())[12] << "\n\n";

	for (int i = 0; i < data->length(); i++) {

		//if (i < 8)
			//hue::grey(std::cout);
		//else if (i < 9)
			//hue::light_purple(std::cout);
		//else if (i < 11)
			//hue::light_blue(std::cout);
		//else if (i < 12)
			//hue::light_purple(std::cout);
		//else if (i < 14)
			//hue::light_blue(std::cout);
		//else if (i < data->length() - 4)
			//hue::reset(std::cout);
		//else
			//hue::green(std::cout);
		

		std::cout << std::hex << (uintptr_t)((uint8_t*)data->data())[i] << " " << std::dec;
	}
	std::cout << "\n";
	*/
	return orig(instance, data);
}