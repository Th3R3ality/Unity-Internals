#include "UnityEngine.Animations.hpp"

#include "Il2cppLib.h"

namespace UnityEngine::Animations{

	int ParentConstraint::AddSource(ConstraintSource* source)
	{
		static auto fn = (int(*)(ParentConstraint*, ConstraintSource*))Il2cppLib::method_from_signature("UnityEngine.Animations::ParentConstraint.AddSource(ConstraintSource)");
		return fn(this, source);
	}
}