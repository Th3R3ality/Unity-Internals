#pragma once
#include "UnityEngine/GameObject/GameObject.hpp"

enum constraint_type
{
	none		= 1 << 0,
	position	= 1 << 1,
	rotation	= 1 << 2
};

class constraint
{
private:
	unsigned int type;
	UnityEngine::GameObject* source;
	UnityEngine::GameObject* destination;
	bool enable;

public:
	constraint() : type(constraint_type::none), source(nullptr), destination(nullptr), enable(false) {};
	constraint(unsigned int type, UnityEngine::GameObject* source, UnityEngine::GameObject* destination, bool enable)
		: type(type), source(source), destination(destination), enable(enable) {};
	
	void apply();
};