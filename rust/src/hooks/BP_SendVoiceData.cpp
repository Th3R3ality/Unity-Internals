#include "hooks.hpp"
#include <iostream>
#include <random>

typedef std::mt19937 rng_type;
std::uniform_int_distribution<rng_type::result_type> udist(5, 10);

rng_type rng;

void hk__BP_SendVoiceData(BasePlayer* instance, System::Array<System::Byte*>* data, int len)
{
	ORIG(hk__BP_SendVoiceData);

	static unsigned int seed = 69; seed += 1;
	rng_type::result_type const seedval = seed;; // get this from somewhere
	rng.seed(seedval);


	static int nigger = 0;

	//std::cout << std::hex << std::endl;

	nigger -= 5;

	for (int i = 32; i < data->length() - 32; i++) {


		//rng_type::result_type random_number = udist(rng);

		/*
		if (!data->data()[i])
			continue;
		*/
		//((uint8_t*)data->data())[i] = random_number;

		//std::cout << (uintptr_t)((uint8_t*)data->data())[i];
	}
	

	//std::cout << std::dec << std::endl << std::endl;
	rng_type::result_type random_number = udist(rng);

	if (nigger <= 25){
		for (int i = 0; i < random_number; i++) {
			nigger += 1;
			orig(instance, data, len);
		}
	}
	return;
}