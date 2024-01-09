#include "BuildingManager.hpp"
#include "Il2cppLib.h"



ClientBuildingManager* BuildingManager::client()
{
	static auto fn = (ClientBuildingManager**)Il2cppLib::static_field_from_signature("BuildingManager.client");
	if (fn)
		return *fn;
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

BuildingManager::Building* BuildingManager::GetBuilding(unsigned int buildingID)
{
	static auto fn = (BuildingManager::Building*(*)(BuildingManager*, unsigned int))Il2cppLib::method_from_signature("BuildingManager.GetBuilding(uint)");
	if (fn)
		return fn(this, buildingID);
	return nullptr;
}

ListHashSet<BuildingBlock*>* BuildingManager::Building::buildingBlocks()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BuildingManager/Building.buildingBlocks");
	if (offset)
		return *(ListHashSet<BuildingBlock*>**)((uintptr_t)this + offset);
	return nullptr;
}

ListHashSet<DecayEntity*>* BuildingManager::Building::decayEntities()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BuildingManager/Building.decayEntities");
	if (offset)
		return *(ListHashSet<DecayEntity*>**)((uintptr_t)this + offset);
	return nullptr;
}

ListHashSet<BuildingPrivlidge*>* BuildingManager::Building::buildingPrivileges()
{
	static auto offset = Il2cppLib::field_offset_from_signature("BuildingManager/Building.buildingPrivileges");
	if (offset)
		return *(ListHashSet<BuildingPrivlidge*>**)((uintptr_t)this + offset);
	return nullptr;
}