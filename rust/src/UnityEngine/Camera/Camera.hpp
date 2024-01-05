#pragma once
#include "UnityEngine/Behaviour/Behaviour.hpp"

#include "UnityEngine/Matrix4x4/Matrix4x4.hpp"

namespace UnityEngine
{
	class Camera : public Behaviour
	{
	public:
		static Camera* main();

		Matrix4x4 worldToCameraMatrix();
		Matrix4x4 projectionMatrix();
		Matrix4x4 nonJitteredProjectionMatrix();
	};
}