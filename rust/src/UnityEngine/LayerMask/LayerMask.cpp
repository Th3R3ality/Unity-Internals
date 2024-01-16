#include "LayerMask.hpp"
#include "Il2cppLib.h"
#include "mscorlib/System/String/String.hpp"
using namespace mscorlib;

namespace UnityEngine
{
    int LayerMask::NameToLayer(std::wstring layer)
    {
        static auto fn = (int(*)(System::String*))Il2cppLib::method_from_signature("UnityEngine::LayerMask.NameToLayer(string)");
        if (fn)
        {
            auto sstr = System::String(layer);
            return fn(&sstr);
        }
        return 0;
    }
}
