#include "hooks.hpp"

#include <iostream>
#include <format>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "cheat.hpp"
#include "config.hpp"
#include "cache.hpp"

#include "UnityEngine/Physics/Physics.hpp"
#include "UnityEngine/LayerMask/LayerMask.hpp"

#include "rust/classes/ClientBuildingManager/ClientBuildingManager.hpp"
#include "rust/classes/BuildingManager/BuildingManager.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"

#include "AstarPathing/Astar.h"

BuildingManager::Building* selectedBuilding = nullptr;

Astar::AstarPath pathfinder(1.f, 0.f, -5, 256u, false, false, false, 1.f, 8, 10.f, 2500u);

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{

	//static int counter = -1; ++counter;

	ORIG(hk__FP_PU_Update);

	if (!orig) {
		std::cout << orig << " : orig much???" << std::endl;
		return;
	}
	orig(instance);
	

	if (cheat::state() == cheat::status::unloading || cheat::state() == cheat::status::none)
	{
		static bool actually_unloading = false;
		if (!actually_unloading) {
			std::cout << "FP_PU unloading\n";
			actually_unloading = true;
			cheat::revert_all_models();
			cheat::unload_assetbundles();
			cheat::has_unloaded(true);
		}
		else
			std::cout << "FP_PU Already Unloaded\n";

		return;
	}

	cache::validatePlayerCache();

	auto localPlayer = cache::local();
	auto mainCam = cache::cameraMain();

	if (localPlayer && mainCam) {
		static int layerMask  (
			UnityEngine::LayerMask::NameToLayer(L"Terrain") |
			UnityEngine::LayerMask::NameToLayer(L"Prevent Movement") |
			UnityEngine::LayerMask::NameToLayer(L"World") |
			UnityEngine::LayerMask::NameToLayer(L"Default") |
			UnityEngine::LayerMask::NameToLayer(L"Construction") |
			UnityEngine::LayerMask::NameToLayer(L"Deployed") |
			UnityEngine::LayerMask::NameToLayer(L"Tree") |
			UnityEngine::LayerMask::NameToLayer(L"Clutter") );

		pathfinder.layerMask = layerMask;

		if (config::PathFinding) {
			///////////////////////////////// WALKER
			static int walkPathIdx = 0;
			static bool hasWalkPoints = false;
			static std::vector<UnityEngine::Vector3> walkPoints;
			static BaseMovement* movement = localPlayer->movement();
			if (!hasWalkPoints)
			{
				walkPathIdx = 0;
				hasWalkPoints = pathfinder.GrabPath(walkPoints);
			}
			else
			{
				if (walkPathIdx < walkPoints.size())
				{
					auto deltaPos = (walkPoints.at(walkPathIdx) - localPlayer->transform()->position());
					//deltaPos.y = 0;
					auto targetMove = UnityEngine::Vector3::Normalize(deltaPos) * 3;

					cache::debugDraw("targetMove", cache::debugLine3d(
						localPlayer->transform()->position(), 
						localPlayer->transform()->position() + targetMove * 0.25, "00ffff"));

					movement->TargetMovement(targetMove);

					if (UnityEngine::Vector3::Distance(localPlayer->transform()->position(), walkPoints.at(walkPathIdx)) < 1.0f)
					{
						walkPathIdx++;
					}
				}
			}

			///////////////////////////////// PATH GEN
			static bool pathing = false;
			static bool newPath = true;
			static UnityEngine::Vector3 startPos, endPos;
			if (GetAsyncKeyState(VK_XBUTTON1) & 0x1) {
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				bool res = UnityEngine::Physics::Raycast(cameraTransform->position(), cameraTransform->forward(), hitInfo, 999.9f, layerMask);
				if (res) {
					auto hitPoint = hitInfo.m_Point;
					hitPoint -= mainCam->transform()->forward() * 0.1;
					startPos = hitPoint;
					std::cout << "start >" << hitPoint << "\n";
					cache::debugDraw("pathStart", cache::debugIcosahedron({ hitPoint, 0,0.1 }, "00ff0066"));

					pathfinder.New(startPos, endPos);
					newPath = true;
					pathing = false;
					hasWalkPoints = false;
				}
			}

			if (GetAsyncKeyState(VK_XBUTTON2) & 0x1)
			{
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				bool res = UnityEngine::Physics::Raycast(cameraTransform->position(), cameraTransform->forward(), hitInfo, 999.9f, layerMask);
				if (res)
				{
					auto hitPoint = hitInfo.m_Point;
					hitPoint -= mainCam->transform()->forward() * 0.1;

					startPos = localPlayer->transform()->position();
					endPos = hitPoint;
					std::cout << "end >" << hitPoint << "\n";
					cache::debugDraw("pathEnd", cache::debugIcosahedron({ hitPoint, 0, 0.1 }, "ff000066"));

					pathfinder.New(startPos, endPos);
					newPath = true;
					pathing = false;
					hasWalkPoints = false;
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


			static bool autoPathing = false;
			static int framesTaken = 0;
			static bool didPrintFramesTaken = true;
			if (pathing == false && didPrintFramesTaken == false)
			{
				std::cout << "pathing took [ " << framesTaken << " ] frames\n";
				didPrintFramesTaken = true;
			}

			if ((autoPathing && newPath) || GetAsyncKeyState(VK_RIGHT) & 0x1) {
				if (newPath) {
					pathing = true;
					newPath = false;
					framesTaken = 0;
					didPrintFramesTaken = false;
				} 
				if (!newPath)
				{
					framesTaken++;
					for (int i = 0; i < frameStepCount; i++)
					{
						pathing = pathfinder.Step(true);
						if (!pathing) break;
					}
				}

			}
			

			if (GetAsyncKeyState(VK_RETURN) & 0x1)
			{
				autoPathing = !autoPathing;
				std::cout << "Toggled Auto Pathing : " << autoPathing << "\n";
			}
			if (autoPathing && pathing)
			{
				framesTaken++;
				for (int i = 0; i < frameStepCount; i++)
				{
					pathing = pathfinder.Step();
					if (!pathing) break;
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