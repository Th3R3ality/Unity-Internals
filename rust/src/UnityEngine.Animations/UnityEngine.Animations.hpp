#pragma once
#include "UnityEngine/Behaviour/Behaviour.hpp"
#include "UnityEngine/Transform/Transform.hpp"

namespace UnityEngine::Animations{
	struct ConstraintSource;
	class ParentConstraint;

	class ParentConstraint : public Behaviour
	{
	public:

		type_overload("UnityEngine.Animations::ParentConstraint");

		int AddSource(ConstraintSource* source);
	};

	struct ConstraintSource
	{
	public:
		Transform* sourceTransform();
		void sourceTransform(Transform* source);

		float weight();
		void weight(float source);

	};
}