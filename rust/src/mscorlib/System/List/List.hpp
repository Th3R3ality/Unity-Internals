#pragma once
#include "mscorlib/System/Array/Array.hpp"
#include "mscorlib/System/Object/Object.hpp"

namespace mscorlib
{
	namespace System
	{
		template<typename t>
		class List
		{
		public:
			char junkBytes[0xF];
			Array<t>* _items; // 0x10
			int _size;
			int _version;
			object* _syncRoot;


			t Get(int i)
			{
				if (i >= _size || i >= _items->length()) {
					return nullptr;
				}
				return _items->data()[i];
			}
		};
	}
}