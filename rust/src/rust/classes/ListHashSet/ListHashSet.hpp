#pragma once
#include "rust/classes/BufferList/BufferList.hpp"
#include "mscorlib/System/Dictionary/Dictionary.hpp"
using namespace mscorlib;

template <typename T>
class ListHashSet
{
	System::Dictionary<T, int>* val2idx; // 0x0
	System::Dictionary<int, T>* idx2val; // 0x0
	BufferList<T>* vals; // 0x0

	BufferList<T>* Values();
	int Count();
};
