#pragma once
#include "Il2cppLib.h"
#include "mscorlib/System/Type/Type.hpp"

namespace mscorlib
{
	namespace System
	{
		class Byte
		{
		public:
			type_overload("System::Byte")
			/*
				__forceinline operator mscorlib::System::Type* ()
			{
				return (mscorlib::System::Type*)Il2cppLib::type_object("System::Byte");
			}
			*/
		};
	}
}