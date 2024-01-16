#pragma once
#include <string>

namespace UnityEngine
{
	struct LayerMask
	{
	public:
		int m_Mask;

		static int NameToLayer(std::wstring layer);
	};
}