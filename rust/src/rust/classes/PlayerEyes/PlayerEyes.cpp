#include "PlayerEyes.hpp"
#include "Il2cppLib.h"

void PlayerEyes::bodyRotation(UnityEngine::Quaternion rotation)
{
    static auto fn = (void(*)(PlayerEyes*, UnityEngine::Quaternion))Il2cppLib::method_from_signature("PlayerEyes.set_bodyRotation(Quaternion)");
    if (fn)
        fn(this, rotation);
}
