#pragma once
#include <string>

#define type_overload(sig) \
	__forceinline operator mscorlib::System::Type* () \
	{ \
		return ( mscorlib::System::Type* )Il2cppLib::type_object(sig); \
	}

#define type_fn(sig) \
	static __forceinline mscorlib::System::Type* type() \
	{ \
		return (mscorlib::System::Type*)Il2cppLib::type_object(sig); \
	}

namespace Il2cppLib
{
	bool initialize();

	void* method_from_signature(std::string signature, bool _virtual = false);
	void* virtual_method_from_signature(std::string signature);

	void* static_field_from_signature(std::string signature);
	uintptr_t field_offset_from_signature(std::string signature);

	void* resolve_icall (const char* name);
	
	void* type_object(std::string class_signature);
	
	void* new_array(std::string class_signature, unsigned int length);

	void* new_object(std::string class_signature);

	std::string _ToString(uintptr_t object);
	
	template<typename T>
	std::string ToString(T object)
	{
		return _ToString((uintptr_t)object);
	}

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