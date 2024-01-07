#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/Vector2/Vector2.hpp"
#include "UnityEngine/Transform/Transform.hpp"

namespace UnityEngine
{
	struct RaycastHit
	{
	public:
		Vector3 m_Point{};
		Vector3 m_Normal{};
		unsigned int m_FaceID{};
		float m_Distance{};
		Vector2 m_UV{};
		int m_Collider{};

		Transform* transform();
	};
}