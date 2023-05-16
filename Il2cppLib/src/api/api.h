#pragma once

#include <stdint.h>

//#include "config-api.h"

//#define DO_API(r, n, p) IL2CPP_EXPORT r n p;

#define expand
#define DO_API(r, n, p) \
	expand typedef r (* __##n ) p; \
	__##n n = ( __##n ) GetProcAddress(Il2cppLib::api::il2cpp_handle, #n)

//#define DO_API( r, n, p ) r n p; //const auto n = GetProcAddress( Il2cppLib::api::il2cpp_handle, n );

//#define DO_API_NO_RETURN(r, n, p) IL2CPP_EXPORT NORETURN r n p;
#define DO_API_NO_RETURN(r, n, p) 

#include <unordered_map>
namespace Il2cppLib::api
{
	const HMODULE il2cpp_handle = GetModuleHandleA("GameAssembly.dll");

	std::unordered_map<std::string, std::string> type_map;
	
	std::string get_type_mapping(std::string real);

	
	void create_type_mappings()
	{
	#define TYPE_MAPPING(mapping, real) type_map[mapping] = real

		TYPE_MAPPING("Vector3", "UnityEngine.Vector3");
		TYPE_MAPPING("Color", "UnityEngine.Color");
		TYPE_MAPPING("Bounds", "UnityEngine.Bounds");
		TYPE_MAPPING("Quaternion", "UnityEngine.Quaternion");
		TYPE_MAPPING("AssetBundle", "UnityEngine.AssetBundle");
		TYPE_MAPPING("Object", "UnityEngine.Object");

		TYPE_MAPPING("float", "System.Single");
		TYPE_MAPPING("bool", "System.Boolean");
		TYPE_MAPPING("string", "System.String");
		TYPE_MAPPING("int", "System.Int32");
		TYPE_MAPPING("object", "System.Object");
		TYPE_MAPPING("Type", "System.Type");

	#undef TYPE_MAPPING
	}
}