#pragma once
#include "rust/classes/BaseMonoBehaviour/BaseMonoBehaviour.hpp"
#include "rust/classes/BasePlayer/BasePlayer.hpp"

class Projectile : public BaseMonoBehaviour
{
public:
	BasePlayer* owner();
};