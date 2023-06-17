#include "Transform.hpp"
#include "UnityEngine/Component/Component.hpp"
#include "mscorlib/System/String/String.hpp"

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
	void Transform::rotation(Quaternion pos)
	{
		static auto fn = (void(*)(Transform*, Quaternion))Il2cppLib::method_from_signature("UnityEngine::Transform.set_rotation(Quaternion)");
		return fn(this, pos);
	}
	Quaternion Transform::rotation()
	{
		static auto fn = (Quaternion(*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_rotation()");
		return fn(this);
	}
	int Transform::childCount()
	{
		static auto fn = (int(*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_childCount()");
		return fn(this);
	}
	Transform* Transform::root()
	{
		static auto fn = (Transform * (*)(Transform*))Il2cppLib::method_from_signature("UnityEngine::Transform.get_root()");
		return fn(this);
	}
	Transform* Transform::GetChild(int index)
	{
		static auto fn = (Transform*(*)(Transform*, int))Il2cppLib::method_from_signature("UnityEngine::Transform.GetChild(int)");
		return fn(this, index);
	}
	Transform* Transform::RecursiveFindChild(Transform* parent, std::wstring name)
	{
		auto count = parent->childCount();
		for (int i = 0; i < count; i++) {
			auto child = parent->GetChild(i);
			auto child_name = child->name();
			if (child_name == name) {
				return child;
			}
			auto ret_ = RecursiveFindChild(child, name);
			if (ret_ != nullptr) {
				return ret_;
			}
		}
		return nullptr;
	}

	Transform* Transform::RecursiveFindChild(Transform* parent, mscorlib::System::String* name)
	{
		return RecursiveFindChild(parent, name->buffer);
	}
}
