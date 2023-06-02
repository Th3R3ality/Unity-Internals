#include "Transform.hpp"
#include "UnityEngine/Component/Component.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	void Transform::position(Vector3 pos)
	{
		static auto fn = (void(*)(Transform*, Vector3))Il2cppLib::method_from_signature("UnityEngine::Transform.set_position(Vector3)");
		return fn(this, pos);
	}
	Vector3 Transform::position()
	{
		static auto fn = (Vector3(*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_position()");
		return fn(this);
	}
	
	void Transform::localPosition(Vector3 pos)
	{
		static auto fn = (void(*)(Transform*, Vector3))Il2cppLib::method_from_signature("UnityEngine::Transform.set_localPosition(Vector3)");
		return fn(this, pos);
	}
	Vector3 Transform::localPosition()
	{
		static auto fn = (Vector3(*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_localPosition()");
		return fn(this);
	}

	void Transform::localRotation(Quaternion pos)
	{
		static auto fn = (void(*)(Transform*, Quaternion))Il2cppLib::method_from_signature("UnityEngine::Transform.set_localRotation(Vector3)");
		return fn(this, pos);
	}
	Quaternion Transform::localRotation()
	{
		static auto fn = (Quaternion(*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_localRotation()");
		return fn(this);
	}
}
