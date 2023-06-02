#include "AssetBundleRequest.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
	Object* AssetBundleRequest::asset()
	{
		static auto fn = (Object*(*)(AssetBundleRequest*))Il2cppLib::method_from_signature("UnityEngine::AssetBundleRequest.get_asset()");
		return fn(this);
	}
}