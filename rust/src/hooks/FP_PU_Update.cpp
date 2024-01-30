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
#include "rust/classes/PlayerInput/PlayerInput.hpp"
#include "rust/classes/PlayerEyes/PlayerEyes.hpp"

#include "RustBot/Pathing.h"

BuildingManager::Building* selectedBuilding = nullptr;

bool autoRepath = false;
bool finishedWalking = false;

RustBot::Pather pathfinder(1.5f, 0.5f, 1.8f, -5, true, 512u, .5f, 5.f, 8, 5.f, 5000u, 1, false);

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
			{
				pathfinder.debugLevel++; pathfinder.debugLevel %= 3;
				std::cout << "debugLevel : " << pathfinder.debugLevel << "\n";
			}

			if (GetAsyncKeyState('O') & 0x1)
			{
				autoRepath = !autoRepath;

				pathfinder.maxPathDepth = autoRepath ? 64u : 512u;
				std::cout << "autoRepath : " << autoRepath << "\n";
				std::cout << "maxPathDepth : " << pathfinder.maxPathDepth << "\n";
			}

			///////////////////////////////// WALKER
			static int walkNodeIdx = 0;
			static bool hasWalkNodes = false;
			static std::vector<UnityEngine::Vector3> walkNodes;
			static BaseMovement* movement = localPlayer->movement();
			auto eyes = localPlayer->eyes();
			auto input = localPlayer->input();

			if (!hasWalkNodes)
			{
				walkNodeIdx = 0;
				hasWalkNodes = pathfinder.GrabPath(walkNodes);
			}
			else
			{
				if (walkNodeIdx < walkNodes.size())
				{
					float totalDistance = UnityEngine::Vector3::Distance(localPlayer->transform()->position() + Vector3(0, pathfinder.radius + 0.1f, 0), walkNodes.at(walkNodeIdx));
					float horizontalDistance = UnityEngine::Vector3::Distance2D(localPlayer->transform()->position() + Vector3(0, pathfinder.radius + 0.1f, 0), walkNodes.at(walkNodeIdx));
					float verticalDistance = std::abs(totalDistance - horizontalDistance);
					auto unitDeltaPos = UnityEngine::Vector3::Normalize(walkNodes.at(walkNodeIdx) - (localPlayer->transform()->position() + Vector3(0, pathfinder.radius + 0.1f ,0)));
					if (unitDeltaPos.y > 0.7) StateMachine::doJump = true;
					
					unitDeltaPos.y = 0;
					UnityEngine::Vector3::Normalize(unitDeltaPos);
					float speedModifier = 1;

					if (verticalDistance > pathfinder.groundedHeight)
					{
						speedModifier = std::clamp(horizontalDistance, 0.1f, 1.f);
					}

					constexpr float moveSpeed = 5.5; // 5.5 run // 2.7 walk
					auto targetMove = unitDeltaPos * std::clamp(speedModifier * moveSpeed, 0.f, moveSpeed);

					cache::debugDraw("walkTargetPos", cache::debugCube(Lapis::Transform(Lapis::Vec3(walkNodes.at(walkNodeIdx)) + Lapis::Vec3(0, 0.3, 0),0,0.05), "00ffff"));

					cache::debugDraw("walkTargetBeacon", cache::debugArrow3d(
						walkNodes.at(walkNodeIdx),
						Lapis::Vec3(0,0.3,0), "00ffff"));
					
					cache::debugDraw("targetMove", cache::debugLine3d(
						localPlayer->transform()->position(),
						localPlayer->transform()->position() + UnityEngine::Vector3::Normalize(targetMove) * speedModifier, "#ee4030"));

					cache::debugDraw("targetMoveBall", cache::debugIcosahedron(Lapis::Transform(
						localPlayer->transform()->position() + UnityEngine::Vector3::Normalize(targetMove) * speedModifier, 0, 0.05f), "#ee403099"));

					auto headPos = localPlayer->transform()->position() + UnityEngine::Vector3(0, 1.8f, 0);
					auto lookPos = localPlayer->transform()->position() + UnityEngine::Vector3::Normalize(targetMove) * speedModifier + UnityEngine::Vector3(0, 0.4f, 0);
					auto deltaPos = UnityEngine::Vector3::Normalize(lookPos - headPos);
					constexpr float rad2deg = static_cast<float>(180.f / M_PI);
					float yaw = std::clamp(atan2(deltaPos.x, deltaPos.z) * rad2deg, -360.f, 360.f);
					float pitch = (asin(-deltaPos.y) * rad2deg);

					auto oldAngles = input->bodyAngles();
					//input->bodyAngles(UnityEngine::Vector3(oldAngles.x, yaw, 0));
					eyes->bodyRotation(UnityEngine::Vector3(pitch, yaw, 0));
					

					movement->TargetMovement(targetMove);

					if (totalDistance < pathfinder.radius)
					{
						walkNodeIdx++;
					}
				}
				else
				{
					finishedWalking = true;
					cache::removeDraw("walkTargetPos");
					cache::removeDraw("walkTargetBeacon");
					cache::removeDraw("targetMove");
					cache::removeDraw("targetMoveBall");
				}
			}

			///////////////////////////////// PATH GEN
			static bool pathing = false;
			static bool newPath = true;
			static UnityEngine::Vector3 startPos, endPos;

			if (autoRepath && finishedWalking && walkNodes.size() > 0)
			{
				std::cout << "repathing\n";
				float direction = static_cast<float>(fastrand() % 360);

				startPos = walkNodes.at(walkNodes.size() - 1);
				endPos = startPos + Vector3(cosf(direction * M_PI / 180) * 200, 0, sinf(direction * M_PI / 180) * 200);
				
				pathfinder.New(startPos, endPos);

				newPath = true;
				pathing = false;
				hasWalkNodes = false;
				finishedWalking = false;
			}

			if (GetAsyncKeyState(VK_XBUTTON1) & 0x1) {
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				if (UnityEngine::Physics::AutoCast(cameraTransform->position(), cameraTransform->forward(), hitInfo, layerMask, 999.9f, pathfinder.radius))
				{
					auto hitPoint = hitInfo.m_Point + hitInfo.m_Normal * pathfinder.radius;
					hitPoint -= mainCam->transform()->forward() * (pathfinder.radius + 0.1f);
					startPos = hitPoint;
					std::cout << "start >" << hitPoint << "\n";
					cache::debugDraw("pathStart", cache::debugIcosahedron({ hitPoint, 0, (pathfinder.radius / 2) }, "00ff0066"));

					pathfinder.New(startPos, endPos);
					newPath = true;
					pathing = false;
					hasWalkNodes = false;
				}
			}

			if (GetAsyncKeyState(VK_XBUTTON2) & 0x1)
			{
				auto cameraTransform = mainCam->transform();

				UnityEngine::RaycastHit hitInfo;
				if (UnityEngine::Physics::AutoCast(cameraTransform->position(), cameraTransform->forward(), hitInfo, layerMask, 999.9f, pathfinder.radius))
				{
					auto hitPoint = hitInfo.m_Point + hitInfo.m_Normal * pathfinder.radius;
					hitPoint -= mainCam->transform()->forward() * 0.1f;

					startPos = localPlayer->transform()->position() + Vector3(0, pathfinder.radius, 0);
					endPos = hitPoint;
					std::cout << "end >" << hitPoint << "\n";
					cache::debugDraw("pathStart", cache::debugIcosahedron({ startPos, 0, (pathfinder.radius / 2) }, "00ff0066"));
					cache::debugDraw("pathEnd", cache::debugIcosahedron({ endPos, 0, (pathfinder.radius / 2) }, "ff000066"));

					pathfinder.New(startPos, endPos);
					newPath = true;
					pathing = false;
					hasWalkNodes = false;
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