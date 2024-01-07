#include "hooks.hpp"

#include <iostream>
#include <Windows.h>

#include "cheat.hpp"
#include "config.hpp"

#include "UnityEngine/Physics/Physics.hpp"

std::vector<uintptr_t> doors;

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{

	//static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
		std::cout << orig << " : orig much???" << std::endl;
		return;
	}
	orig(instance);

	static bool actually_unloading = false;
	if (!actually_unloading && cheat::state() == cheat::status::unloading) {
		actually_unloading = true;
		cheat::revert_all_models();
		cheat::unload_assetbundles();
		cheat::has_unloaded(true);
		return;
	}

	cache::validatePlayerCache();


	auto localPlayer = cache::local();
	auto mainCam = cache::cameraMain();

	if (localPlayer && mainCam) {
		
		if (GetAsyncKeyState('I') & 0x1) {
			auto cameraTransform = mainCam->transform();

			UnityEngine::RaycastHit hitInfo;

			bool res = UnityEngine::Physics::Raycast(cameraTransform->position(), cameraTransform->forward(), hitInfo);
			auto hitTransform = hitInfo.transform();

			if (res && hitTransform) {

				auto hitObjectPos = hitTransform->position();


				std::wcout << hitTransform->name() << "\n";
				std::cout << "Pos : " << hitObjectPos << "\n";
				cache::debugDraw("hitObject", cache::debugDrawable(Lapis::Transform(hitObjectPos, 0, 0.1f), "0050ff55", Lapis::Shape::Icosahedron));

				if (*hitTransform->name() == L"hinge") {
					auto hatchPos = hitTransform->GetChild(1)->position();

					cache::debugDraw("hatch", cache::debugDrawable(Lapis::Transform(Lapis::Vec3(hatchPos) + Lapis::Vec3::up*0.1f, 0, 0.1f), "ff005055", Lapis::Shape::Icosahedron));
				}
				
			}
		}
	}
}