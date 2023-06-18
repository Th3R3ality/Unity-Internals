#pragma once
#include "UnityEngine/Component/Component.hpp"
#include "UnityEngine/Vector3/Vector3.hpp"


namespace UnityEngine
{
	class Transform : public Component
	{
	public:
		void position(Vector3 pos);
		Vector3 position();

		void localPosition(Vector3 pos);
		Vector3 localPosition();

		void rotation(Quaternion pos);
		Quaternion rotation();
		
		void localRotation(Quaternion pos);
		Quaternion localRotation();

		int childCount();
		Transform* root();
		Transform* GetChild(int index);

		static Transform* RecursiveFindChild(Transform* parent, std::wstring name);
		static Transform* RecursiveFindChild(Transform* parent, mscorlib::System::String* name);

	};
}