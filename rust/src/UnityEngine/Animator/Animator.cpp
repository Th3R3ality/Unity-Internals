#include "Animator.hpp"

#include "Il2cppLib.h"
#include <iostream>

namespace UnityEngine {
	Avatar* Animator::avatar()
	{
		static auto fn = Il2cppLib::resolve_icall("UnityEngine.Animator::get_avatar()");
		return ((Avatar * (*)(Animator*))fn)(this);
	}

	void Animator::avatar(Avatar* avatar)
	{
		static auto fn = Il2cppLib::resolve_icall("UnityEngine.Animator::set_avatar(UnityEngine.Avatar)");
		return ((void (*)(Animator*, Avatar*))fn)(this, avatar);
	}

	UnityEngine::Transform* Animator::GetBoneTransform(HumanBodyBones bone)
	{
		static auto fn = Il2cppLib::resolve_icall("UnityEngine.Animator::GetBoneTransformInternal(System.Int32)");
		if (fn)
			return ((UnityEngine::Transform * (*)(Animator*, int))fn)(this, bone);
		return nullptr;
	}
}