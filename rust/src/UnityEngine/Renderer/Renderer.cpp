#include "Renderer.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
    void Renderer::enabled(bool value)
    {
        static auto fn = (void(*)(Renderer*, bool))Il2cppLib::method_from_signature("UnityEngine::Renderer.set_enabled(bool)");
        return fn(this, value);
    }
}