#include "Camera.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	Camera* Camera::main()
	{
		auto fn = (Camera*(*)())Il2cppLib::method_from_signature("UnityEngine::Camera.get_main()");
		if (fn)
			return fn();
		return nullptr;
	}

	Matrix4x4 Camera::worldToCameraMatrix()
	{
		auto fn = (Matrix4x4(*)(Camera*))Il2cppLib::method_from_signature("UnityEngine::Camera.get_worldToCameraMatrix()");

		if (fn)
			return fn(this);
		return {};
	}

	Matrix4x4 Camera::projectionMatrix()
	{
		auto fn = (Matrix4x4(*)(Camera*))Il2cppLib::method_from_signature("UnityEngine::Camera.get_projectionMatrix()");

		if (fn)
			return fn(this);
		return {};
	}

	Matrix4x4 Camera::nonJitteredProjectionMatrix()
	{
		auto fn = (Matrix4x4(*)(Camera*))Il2cppLib::method_from_signature("UnityEngine::Camera.get_nonJitteredProjectionMatrix()");

		if (fn)
			return fn(this);
		return {};
	}
}