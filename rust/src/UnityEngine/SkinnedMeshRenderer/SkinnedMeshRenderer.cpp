#include "UnityEngine/SkinnedMeshRenderer/SkinnedMeshRenderer.hpp"

namespace UnityEngine
{
    Transform* SkinnedMeshRenderer::rootBone()
    {
        static auto fn = (Transform*(*)(SkinnedMeshRenderer*))Il2cppLib::method_from_signature("UnityEngine::SkinnedMeshRenderer.get_rootBone()");
        return fn(this);
    }
}