#pragma once
#include "UnityEngine/ResourceRequest/ResourceRequest.hpp"
#include "UnityEngine/Object/Object.hpp"

namespace UnityEngine
{
	class AssetBundleRequest : public ResourceRequest
	{
	public:

		Object* asset();
	};
}