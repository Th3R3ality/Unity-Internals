#pragma once
#include "rust/classes/BaseNetworkable/BaseNetworkable.hpp"
#include "rust/classes/Model/Model.hpp"
#include "rust/classes/BufferList/BufferList.hpp"

class BaseEntity : public BaseNetworkable
{
public:
	Model* model();

};