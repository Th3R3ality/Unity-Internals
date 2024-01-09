#pragma once
#include "rust/classes/ListHashSet/ListHashSet.hpp"
#include "rust/classes/ListDictionary/ListDictionary.hpp"
#include "rust/classes/DecayEntity/DecayEntity.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"
#include "rust/classes/BuildingPrivlidge/BuildingPrivlidge.hpp"

#include "Il2cppLib.h"

class ClientBuildingManager;
class BuildingManager
{
public:
	class Building;

	static ClientBuildingManager* client(); //static getter

	ListHashSet<DecayEntity*>* decayEntities(); //getter
	ListDictionary<unsigned int, BuildingManager::Building*>* buildingDictionary(); //getter

	Building* GetBuilding(unsigned int buildingID);

	class Building
	{
	public:
		type_fn("BuildingManager.Building");

		ListHashSet<BuildingBlock*>* buildingBlocks();
		ListHashSet<DecayEntity*>* decayEntities();
		ListHashSet<BuildingPrivlidge*>* buildingPrivileges();
	};

};