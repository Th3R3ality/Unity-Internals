#pragma once
#include <ostream>

namespace mscorlib
{
	namespace System
	{
		class String
		{
		public:
			char pad[0x10]{ };
			int size{ };
			wchar_t buffer[128 + 1];

		public:
			String(const wchar_t* str);

			bool startsWith(std::string comparison);

			std::string rest(int start);

			friend std::wostream& operator<<(std::wostream& os, const System::String* Sstr);
		};
	}
}