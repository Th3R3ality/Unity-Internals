#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"

namespace UnityEngine
{
	struct Quaternion
	{
	public:
		float x, y, z, w;

		static Quaternion identity();
		static Quaternion Euler(Vector3 euler);

		friend std::ostream& operator<<(std::ostream& os, const Quaternion& q)
		{
			os << q.x << '/' << q.y << '/' << q.z << '/' << q.w;
			return os;
		}
	};
}