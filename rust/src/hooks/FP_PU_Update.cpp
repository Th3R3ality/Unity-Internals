#include "hooks.hpp"

#include <iostream>
#include <format>
#include <Windows.h>

#include "cheat.hpp"
#include "config.hpp"
#include "cache.hpp"

#include "UnityEngine/Physics/Physics.hpp"

#include "rust/classes/ClientBuildingManager/ClientBuildingManager.hpp"
#include "rust/classes/BuildingManager/BuildingManager.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"

#include "AstarPathing/Astar.h"

BuildingManager::Building* selectedBuilding = nullptr;

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
		
		if (config::PathFinding) {
			static bool pathing = false;
			static bool newPath = true;
			static UnityEngine::Vector3 startPos, endPos;
			if (GetAsyncKeyState(VK_XBUTTON1) & 0x1) {
				std::cout << "pressed I\n";
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				bool res = UnityEngine::Physics::Raycast(cameraTransform->position(), cameraTransform->forward(), hitInfo);

				if (res) {
					static bool start = false;
					start = !start;

					auto hitPoint = hitInfo.m_Point;
					hitPoint -= mainCam->transform()->forward() * 0.1;
					if (start) {
						startPos = hitPoint;
						std::cout << "start >" << hitPoint << "\n";
						cache::debugDraw("pathStart", cache::debugIcosahedron({ hitPoint, 0,0.1 }, "00ff0066"));
					}
					else {
						endPos = hitPoint;
						endPos.y = startPos.y;
						std::cout << "end >" << hitPoint << "\n";
						cache::debugDraw("pathEnd", cache::debugIcosahedron({ hitPoint, 0, 0.1 }, "ff000066"));
					}
					newPath = true;
					pathing = false;
				}
			}
			static int frameStepCount = 1;
			if (GetAsyncKeyState(VK_UP) & 0x1)
			{
				frameStepCount++; std::cout << "new steps/frame : " << frameStepCount << "\n";
			}


			if (GetAsyncKeyState(VK_DOWN) & 0x1)
			{
				frameStepCount--; std::cout << "new steps/frame : " << frameStepCount << "\n";
			}

			if (GetAsyncKeyState(VK_RIGHT) & 0x1) {
				if (newPath) {
					Astar::New(startPos, endPos);
					pathing = true;
					newPath = false;
				} 
				else
				{
					Astar::Step();
				}

			}

			constexpr bool autoPathing = true;
			if (autoPathing && pathing)
			{
				for (int i = 0; i < frameStepCount; i++)
				{
					pathing = Astar::Step();
				}
			}
		}

		if (config::BaseAnalyzer && GetAsyncKeyState('I') & 0x1) {
			auto cameraTransform = mainCam->transform();

			UnityEngine::RaycastHit hitInfo;

			bool res = UnityEngine::Physics::Raycast(cameraTransform->position(), cameraTransform->forward(), hitInfo);
			auto hitTransform = hitInfo.transform();

			if (res && hitTransform) {

				std::wcout << L"hit > " << hitTransform->name() << "\n";
				cache::debugDraw("hitObject", cache::debugIcosahedron(Lapis::Transform(hitTransform->position(), 0, 0.1f), "0050ff55"));

				auto buildingComponent = (DecayEntity*)hitTransform->root()->GetComponent(DecayEntity::type());

				if (buildingComponent) {
					auto buildId = buildingComponent->buildingID();
					std::cout << std::format("Building ID : {}\n", buildId);
					
					auto clientBuildingManager = BuildingManager::client();
					std::cout << std::format("clientBuildingManager : {:x}\n", (uintptr_t)clientBuildingManager);

					auto building = clientBuildingManager->GetBuilding(buildId);
					std::cout << std::format("building : {:x}\n", (uintptr_t)building);

					cache::ptr("building", building);
				}
			}
		}
	}
}