#include "Model.hpp"
#include "Il2cppLib.h"

UnityEngine::Animator* Model::animator()
{
    static auto fn = Il2cppLib::field_offset_from_signature("Model.animator");
    if (fn)
        return *(UnityEngine::Animator **)((uintptr_t)this + fn);
    return nullptr;
}

System::Array<UnityEngine::Transform*>* Model::boneTransforms()
{
    static auto fn = Il2cppLib::field_offset_from_signature("Model.boneTransforms");
    if (fn)
        return *(System::Array<UnityEngine::Transform*>**)((uintptr_t)this+ fn);
    return nullptr;
}
