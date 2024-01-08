#include "BuildingManager.hpp"
#include "Il2cppLib.h"

ClientBuildingManager* BuildingManager::client()
{
	static auto fn = (ClientBuildingManager*(*)())Il2cppLib::method_from_signature("BuildingManager.get_client()");
	if (fn)
		return fn();
	return nullptr;
}

ListHashSet<DecayEntity*>* BuildingManager::decayEntities()
{
	static auto fn = (ListHashSet<DecayEntity*>*(*)(BuildingManager*))Il2cppLib::method_from_signature("BuildingManager.get_decayEntities()");
	if (fn)
		return fn(this);
	return nullptr;
}

ListDictionary<unsigned int, BuildingManager::Building*>* BuildingManager::buildingDictionary()
{
	static auto fn = (ListDictionary<unsigned int, BuildingManager::Building*>*(*)(BuildingManager*))Il2cppLib::method_from_signature("BuildingManager.get_buildingDictionary()");
	if (fn)
		return fn(this);
	return nullptr;
}
