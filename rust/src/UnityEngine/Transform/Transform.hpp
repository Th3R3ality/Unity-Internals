#pragma once
#include "UnityEngine/Component/Component.hpp"
#include "UnityEngine/Vector3/Vector3.hpp"

#define _transform(x) ((UnityEngine::Transform*)x->transform())

namespace UnityEngine
{
	class Transform : public Component
	{
	public:
		void position(Vector3 pos);
		Vector3 position();

		void localPosition(Vector3 pos);
		Vector3 localPosition();

		void localRotation(Quaternion pos);
		Quaternion localRotation();
		
	};
}