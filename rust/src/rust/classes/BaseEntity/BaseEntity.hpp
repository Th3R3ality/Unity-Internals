#pragma once
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"

#include "rust/classes/Model/Model.hpp"

class BaseEntity : public BaseNetworkable
{
public:
	Model* model();
};