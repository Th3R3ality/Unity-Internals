#pragma once
#include "UnityEngine/MonoBehaviour/MonoBehaviour.hpp"
#include "UnityEngine/Transform/Transform.hpp"
#include "mscorlib/System/Array/Array.hpp"
#include "UnityEngine/Animator/Animator.hpp"

class Model : public UnityEngine::MonoBehaviour
{
public:
	UnityEngine::Animator* animator();
	System::Array<UnityEngine::Transform*>*boneTransforms();
};