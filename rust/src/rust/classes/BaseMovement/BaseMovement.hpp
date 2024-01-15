#pragma once
#include "UnityEngine/MonoBehaviour/MonoBehaviour.hpp"
#include "UnityEngine/Vector3/Vector3.hpp"

class BaseMovement : public UnityEngine::MonoBehaviour
{
public:

	void TargetMovement(UnityEngine::Vector3 v3);
};