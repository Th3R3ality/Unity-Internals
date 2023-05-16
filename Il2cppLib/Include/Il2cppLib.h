#pragma once
#include <string>

namespace Il2cppLib
{
	bool initialize();

	void* method_from_signature(std::string signature, bool _virtual = false);
	void* virtual_method_from_signature(std::string signature);

	void* resolve_icall (const char* name);
	
	void* type_object(std::string class_signature);
	
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