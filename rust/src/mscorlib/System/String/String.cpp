#include "String.hpp"

#include <iostream>

#define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

namespace mscorlib::System
{		
	String::String(std::wstring str)
	{
		size = min(str.length(), 128);
		for (int idx = 0; idx < size; idx++) {
			buffer[idx] = str.at(idx);
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
	bool operator==(const String* self, const std::wstring& other)
	{
		std::wcout << L"comparing: " << self->buffer << L" == " << other << " = " << (other.compare(self->buffer) == 0) << std::endl;
		return other.compare(self->buffer) == 0;
	}

}