#include "String.hpp"

namespace mscorlib::System
{		
	String::String(const wchar_t* str)
	{
#define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
		size = min(wcslen(str), 128);
#undef min
		for (int idx = 0; idx < size; idx++) {
			buffer[idx] = str[idx];
		}
		buffer[size] = 0;
	}

	bool String::startsWith(std::string comparison)
	{
		std::wstring ws(comparison.begin(), comparison.end());
		for (size_t idx = 0; idx < comparison.length(); idx++) {
			if (buffer[idx] != comparison.at(idx))
				return false;
		}
		return true;
	}

	std::string String::rest(int start)
	{
		std::wstring ws(buffer);
		std::wstring result = ws.substr(start);
		std::string str(result.begin(), result.end());
		return str;
	}

	std::wostream& operator<<(std::wostream& os, const System::String* Sstr)
	{
		os << Sstr->buffer;
		return os;
	}	
}