#pragma once
#include "rust/classes/BaseMonoBehaviour/BaseMonoBehaviour.hpp"
#include "ProtoBuf/Entity/Entity.hpp"

class BaseNetworkable : public BaseMonoBehaviour
{
public:
	struct LoadInfo
	{
	public:
		ProtoBuf::Entity* msg;
		bool fromDisk;
	};
};