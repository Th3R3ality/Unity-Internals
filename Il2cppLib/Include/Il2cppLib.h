#pragma once

namespace Il2cppLib
{
	void initialize();

	void* method_from_signature(std::string signature);

	namespace api
	{
		void* _new_string(std::string str);

		template<typename t>
		t new_string(std::string str)
		{
			return (t)_new_string(str);
		}
	}
}