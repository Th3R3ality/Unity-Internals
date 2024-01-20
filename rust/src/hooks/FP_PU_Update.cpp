#include "hooks.hpp"

#include <iostream>
#include <format>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "cheat.hpp"
#include "config.hpp"
#include "cache.hpp"

#include "Util/math_util.h"

#include "globalSM.h"

#include "UnityEngine/Physics/Physics.hpp"
#include "UnityEngine/LayerMask/LayerMask.hpp"

#include "rust/classes/ClientBuildingManager/ClientBuildingManager.hpp"
#include "rust/classes/BuildingManager/BuildingManager.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"

#include "AstarPathing/Astar.h"

BuildingManager::Building* selectedBuilding = nullptr;

bool autoRepath = true;
bool finishedWalking = false;

constexpr float pathRadius = 0.4f;
Astar::AstarPath pathfinder(1.f, pathRadius, -5, true, 256u, false, .5f, 3.0f, 8, 8.f, 5000u, 1, false, false);

void hk__FP_PU_Update(Facepunch::PerformanceUI* instance)
{
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
			UnityEngine::LayerMask::NameToLayer(L"Clutter") |
			UnityEngine::LayerMask::NameToLayer(L"Transparent"));

		pathfinder.layerMask = layerMask;

		if (config::PathFinding) {
			if (GetAsyncKeyState('P') & 0x1)
				pathfinder.debugLevel++; pathfinder.debugLevel %= 3;

			if (GetAsyncKeyState('O') & 0x1)
			{
				autoRepath = !autoRepath;
				std::cout << "autoRepath : " << autoRepath << "\n";
			}

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
					auto deltaPos = UnityEngine::Vector3::Normalize(walkPoints.at(walkPathIdx) - (localPlayer->transform()->position() + Vector3(0,pathfinder.radius + 0.1f,0)));
					if (deltaPos.y > 0.6) StateMachine::doJump = true;
					deltaPos.y = 0;
					auto targetMove = deltaPos * 5.5f; //2.7 walk bruh

					cache::debugDraw("targetMove", cache::debugLine3d(
						localPlayer->transform()->position() + Vector3(0, pathRadius + 0.1f, 0),
						localPlayer->transform()->position() + Vector3(0, pathRadius + 0.1f, 0) + targetMove * 0.25, "00ffff"));

					movement->TargetMovement(targetMove);

					if (UnityEngine::Vector3::Distance(localPlayer->transform()->position() + Vector3(0,pathRadius + 0.1f, 0), walkPoints.at(walkPathIdx)) < pathRadius*1.75)
					{
						walkPathIdx++;
					}
				}
				else
				{
					finishedWalking = true;
				}
			}

			///////////////////////////////// PATH GEN
			static bool pathing = false;
			static bool newPath = true;
			static UnityEngine::Vector3 startPos, endPos;

			if (autoRepath && finishedWalking)
			{
				std::cout << "repathing\n";
				float direction = static_cast<float>(fastrand() % 360);

				startPos = walkPoints.at(walkPoints.size() - 1);
				endPos = Vector3(cosf(direction * M_PI / 180) * 10000, startPos.y, sinf(direction * M_PI / 180) * 10000);
				
				pathfinder.New(startPos, endPos);

				newPath = true;
				pathing = false;
				hasWalkPoints = false;
				finishedWalking = false;
			}

			if (GetAsyncKeyState(VK_XBUTTON1) & 0x1) {
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				if (UnityEngine::Physics::AutoCast(cameraTransform->position(), cameraTransform->forward(), hitInfo, layerMask, 999.9f, pathRadius))
				{
					auto hitPoint = hitInfo.m_Point + hitInfo.m_Normal * pathRadius;
					hitPoint -= mainCam->transform()->forward() * (pathRadius+0.1f);
					startPos = hitPoint;
					std::cout << "start >" << hitPoint << "\n";
					cache::debugDraw("pathStart", cache::debugIcosahedron({ hitPoint, 0, (pathRadius) }, "00ff0066"));

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
				if (UnityEngine::Physics::AutoCast(cameraTransform->position(), cameraTransform->forward(), hitInfo, layerMask, 999.9f, pathRadius))
				{
					auto hitPoint = hitInfo.m_Point + hitInfo.m_Normal * pathRadius;
					hitPoint -= mainCam->transform()->forward() * 0.1f;

					startPos = localPlayer->transform()->position() + Vector3(0, (pathRadius + 0.1f), 0);
					endPos = hitPoint;
					std::cout << "end >" << hitPoint << "\n";
					cache::debugDraw("pathStart", cache::debugIcosahedron({ startPos, 0, (pathRadius) }, "00ff0066"));
					cache::debugDraw("pathEnd", cache::debugIcosahedron({ endPos, 0, (pathRadius) }, "ff000066"));

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