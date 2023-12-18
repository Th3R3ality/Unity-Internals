#include "Renderer.hpp"

#include "Il2cppLib.h"

namespace UnityEngine
{
    void Renderer::enabled(bool value)
    {
        static auto fn = (void(*)(Renderer*, bool))Il2cppLib::method_from_signature("UnityEngine::Renderer.set_enabled(bool)");
        return fn(this, value);
    }

    mscorlib::System::Array<Material*>* Renderer::materials()
    {
        static auto fn = (mscorlib::System::Array<Material*>*(*)(Renderer*))Il2cppLib::method_from_signature("UnityEngine::Renderer.get_materials()");
        return fn(this);
    }
}