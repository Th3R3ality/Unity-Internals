#include "UnityEngine.Animations.hpp"

#include "Il2cppLib.h"

namespace UnityEngine::Animations
{

	Transform* ConstraintSource::sourceTransform()
	{
		static auto fn = (Transform*(*)(ConstraintSource*))Il2cppLib::method_from_signature("UnityEngine.Animations::ConstraintSource.get_sourceTransform()");
		return fn(this);

	}
	void ConstraintSource::sourceTransform(Transform* source)
	{
		static auto fn = (void(*)(ConstraintSource*, Transform*))Il2cppLib::method_from_signature("UnityEngine.Animations::ConstraintSource.set_sourceTransform()");
		return fn(this, source);
	}

	float ConstraintSource::weight()
	{
		static auto fn = (float(*)(ConstraintSource*))Il2cppLib::method_from_signature("UnityEngine.Animations::ConstraintSource.get_weight()");
		return fn(this);

	}

	void ConstraintSource::weight(float source)
	{
		static auto fn = (void(*)(ConstraintSource*, float))Il2cppLib::method_from_signature("UnityEngine.Animations::ConstraintSource.set_weight()");
		return fn(this, source);
	}
}