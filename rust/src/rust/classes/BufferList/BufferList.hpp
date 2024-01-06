#pragma once
#include "mscorlib/System/Array/Array.hpp"

template <typename t>
class BufferList
{
public:
	char zeroBytes[0xf];
	int count;
	mscorlib::System::Array<t>* buffer;

	t Get(int idx)
	{
		if (idx >= count && idx >= buffer->length())
			return nullptr;
		return buffer->data()[idx];
	}
};