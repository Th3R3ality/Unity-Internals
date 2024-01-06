#include "hooks.hpp"
#include <iostream>
#include <format>

#include "cache.hpp"

#include "rust/classes/BaseProjectile/BaseProjectile.hpp"

typedef void(*Il2CppMethodPointer)();
typedef void (*InvokerMethod)(Il2CppMethodPointer, const MethodInfo*, void*, void**, void*);
typedef struct MethodInfo
{
    void* methodPointer;
    void* virtualMethodPointer;
    InvokerMethod invoker_method;
    const char* name;
    /*Il2CppClass*/void* klass;
    const /*Il2CppType*/void* return_type;
    const /*Il2CppType*/void** parameters;
    union
    {
        const /*Il2CppRGCTXData*/void* rgctx_data;
        //Il2CppMetadataMethodDefinitionHandle methodMetadataHandle;
    };
    union
    {
        const /*Il2CppGenericMethod*/void* genericMethod;
        //Il2CppMetadataGenericContainerHandle genericContainerHandle;
    };
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint8_t parameters_count;
    uint8_t is_generic : 1;
    uint8_t is_inflated : 1;
    uint8_t wrapper_type : 1;
    uint8_t has_full_generic_sharing_signature : 1;
    uint8_t indirect_call_via_invokers : 1;
} MethodInfo;

void hk__projectile_shoot(BaseEntity* instance, mscorlib::System::String* funcName, uint64_t arg1, const MethodInfo* method)
{
    //std::cout << "projectile shoot\n";

	ORIG(hk__projectile_shoot);

    try {
        std::wcout << "funcName : " << funcName << std::endl;
        if (funcName->equals(L"CLProject")) {
            std::cout << "instance : " << instance << "\n";
            auto projectiles = ((BaseProjectile*)instance)->createdProjectiles();

            auto lp = cache::local();
            auto cam = cache::cameraMain();
            if (lp && cam) {

                auto camPos = cam->transform()->position();
                auto pos = camPos + UnityEngine::Vector3(0, 1, 0);
                cache::debugDraw("manip", cache::debugDrawable(Lapis::Transform(pos, 0, 0.1), "fff33399", Lapis::Shape::Icosahedron));


                std::cout << "did yo crash yet?\n";

                for (int i = 0; i < projectiles->_size; i++) {
                    projectiles->Get(i)->transform()->position(pos);
                }
                std::cout << "did yo crash nyow??\n";
            }
        }
    }
    catch (std::exception e){
        std::cout << "exception in projectile_shoot hook\n";
    }

    orig(instance, funcName, arg1, method);
}