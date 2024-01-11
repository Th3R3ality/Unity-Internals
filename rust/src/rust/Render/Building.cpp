#include "Render.h"

#include <vector>

#include "Il2cppLib.h"

#include "cache.hpp"
#include "rust/classes/BuildingManager/BuildingManager.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"
#include "rust/classes/Door/Door.hpp"
#include "Lapis/engine/LapisEngine.h"

#include "Lapis/utils/hsl-to-rgb.hpp"

namespace Render
{
	void Building()
	{
		static BuildingManager::Building* oldBuilding = nullptr;
		auto building = cache::ptr<BuildingManager::Building*>("building");
		if (!building) return;

		auto decayEntitiesListHashSet = building->decayEntities();

		auto decayEnts = decayEntitiesListHashSet->vals;

		for (int i = 0; i < decayEnts->count; i++) {
			auto decayEnt = decayEnts->Get(i);

			std::string type = Il2cppLib::ToString(decayEnt);

			auto entityPos = decayEnt->transform()->position();

			Door* door = nullptr;
			if (type == "Door") {
				door = (Door*)decayEnt;
			}

			if (door && door->hasHatch()) {

				auto model = door->model();
				auto bones = model->boneTransforms();
				auto animator = model->animator();

				if (animator->name()->equals(L"double_door_rig")) {
					entityPos = bones->Get(4)->position() + UnityEngine::Vector3(0,0.1,0);
					Lapis::Draw::D3::Icosahedron(Lapis::Transform(entityPos, 0, 0.15), "ff005099");
					entityPos = bones->Get(17)->position() + UnityEngine::Vector3(0, 0.1, 0);
					Lapis::Draw::D3::Icosahedron(Lapis::Transform(entityPos, 0, 0.15), "ff005099");
				} else {
					entityPos = bones->Get(4)->position() + UnityEngine::Vector3(0, 0.1, 0);
					Lapis::Draw::D3::Icosahedron(Lapis::Transform(entityPos, 0, 0.15), "00ff5099");
				}
			}
			else {

				//Lapis::Draw::D3::Cube(Lapis::Transform(entityPos, 0, 0.07), hsl2rgb(fmod((Lapis::elapsedTime * 10 + i * 5), 360), .8f, .5f, .7f));
			}
		}
		oldBuilding = building;
	}
}