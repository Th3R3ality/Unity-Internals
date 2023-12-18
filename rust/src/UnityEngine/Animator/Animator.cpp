#include "Animator.hpp"

#include "Il2cppLib.h"
#include <iostream>

namespace UnityEngine {
	Avatar* Animator::avatar()
	{
		//static auto fn = (Avatar*(*)(Animator*))Il2cppLib::method_from_signature("UnityEngine::Animator.get_avatar()");
		//return fn(this);
		
		//static auto _fn = (Avatar*(*)(Animator*))Il2cppLib::method_from_signature("UnityEngine::Animator.get_avatar()");
		//std::cout << "get_avatar Il2cppLib: " << (void*)_fn;
		
		static auto fn = Il2cppLib::resolve_icall("UnityEngine.Animator::get_avatar()");
		//std::cout << "get_avatar resolve_icall: " << (void*)fn;

		return ((Avatar * (*)(Animator*))fn)(this);
	}

	void Animator::avatar(Avatar* avatar)
	{
		//static auto fn = (void (*)(Animator*, Avatar*))Il2cppLib::method_from_signature("UnityEngine::Animator.set_avatar(UnityEngine::Avatar)");
		//return fn(this, avatar);

		//static auto _fn = (void (*)(Animator*, Avatar*))Il2cppLib::method_from_signature("UnityEngine::Animator.set_avatar(UnityEngine::Avatar)");
		//std::cout << "set_avatar Il2cppLib: " << (void*)_fn;

		static auto fn = Il2cppLib::resolve_icall("UnityEngine.Animator::set_avatar(UnityEngine.Avatar)");
		//std::cout << "set_avatar resolve_icall: " << (void*)fn;

		return ((void (*)(Animator*, Avatar*))fn)(this, avatar);
	}
}