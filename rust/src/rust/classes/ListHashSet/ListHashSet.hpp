#pragma once
#include "rust/classes/BufferList/BufferList.hpp"
#include "mscorlib/System/Dictionary/Dictionary.hpp"
using namespace mscorlib;

#include "Il2cppLib.h"


#include <iostream>
template <typename T>
class ListHashSet
{
public:
	char junkBytes[0xf];
	System::Dictionary<T, int>* val2idx; // 0x10
	System::Dictionary<int, T>* idx2val; // 0x18
	BufferList<T>* vals; // 0x20
};
