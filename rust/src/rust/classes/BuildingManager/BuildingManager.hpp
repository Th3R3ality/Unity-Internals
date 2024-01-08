#pragma once
#include "rust/classes/ClientBuildingManager/ClientBuildingManager.hpp"
#include "rust/classes/ListHashSet/ListHashSet.hpp"
#include "rust/classes/ListDictionary/ListDictionary.hpp"
#include "rust/classes/DecayEntity/DecayEntity.hpp"

class ClientBuildingManager;

class BuildingManager
{
public:
	class Building;


	static ClientBuildingManager* client(); //static getter
	ListHashSet<DecayEntity*>* decayEntities(); //getter
	ListDictionary<unsigned int, BuildingManager::Building*>* buildingDictionary(); //getter


	class Building
	{
	public:

	};

};