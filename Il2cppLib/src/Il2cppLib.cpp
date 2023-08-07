// Il2cppLib.cpp : Defines the functions for the static library.
//

#include "pch.h"

#include "../Include/Il2cppLib.h"

#include "api/2021.3.21f1.h"



namespace Il2cppLib
{
	Il2CppClass* class_from_signature(std::string signature);
	std::string api::get_type_mapping(std::string real);

	bool initialize()
	{
		api::create_type_mappings();

		return true;
	}

	void* type_object(std::string class_signature)
	{
		const Il2CppType* type = il2cpp_class_get_type(class_from_signature(class_signature));
		return il2cpp_type_get_object(type);
	}

	void* new_object(std::string class_signature)
	{
		return il2cpp_object_new(class_from_signature(class_signature));
	}

	void* resolve_icall(const char* name)
	{
		return (void*)il2cpp_resolve_icall(name);
	}

	Il2CppClass* class_from_signature(std::string signature)
	{

		std::string name;
		std::string namespaze = "";

		size_t pos = signature.find_last_of("::");
		if (pos != std::string::npos) {
			namespaze = signature.substr(0, pos - 1);
			signature.erase(0, pos + 1);
		}

		name = signature;

		auto domain = il2cpp_domain_get();
		size_t nrofassemblies = 0;
		const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(domain, &nrofassemblies);

		for (int i = 0; i < nrofassemblies; i++) {
			const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);

			Il2CppClass* kl = il2cpp_class_from_name(image, namespaze.c_str(), name.c_str());
			if (!kl) continue;

			return kl;
		}
		return 0;
	}

	void* method_from_signature(std::string signature, bool _virtual)
	{
		std::string klass_signature, method_name;
		size_t pos = 0;
		int arg_count = -1;

		pos = signature.find(".");
		if (pos != std::string::npos) {
			klass_signature = signature.substr(0, pos);
			signature.erase(0, pos + 1);
		}
		else {
			return 0;
		}
		auto klass = class_from_signature(klass_signature);
		if (!klass) return 0;

		pos = signature.find("(");
		if (pos != std::string::npos) {
			method_name = signature.substr(0, pos);
			signature.erase(0, pos + 1);

			pos = signature.find(")");
			if (pos != std::string::npos) {
				arg_count = pos;
				signature.at(pos) = ',';
			}
		}
		
		void* iterator{ 0 };
		while (const MethodInfo* method = il2cpp_class_get_methods(klass, &iterator)) {
			if (method_name.compare(method->name)) continue;
			
			
			int param_idx = 0;
			std::string parameters = signature;
			
			while (parameters.find(',') != std::string::npos && arg_count > 0) {
				param_idx += 1;
				if (param_idx > method->parameters_count) break;
				std::string sig_typename = [&]() -> std::string { size_t pos = parameters.find(','); auto chunk = parameters.substr(0, pos); parameters.erase(0, pos + 1); return chunk; }();
				
				if (api::get_type_mapping(sig_typename) != "none")
					sig_typename = api::get_type_mapping(sig_typename);

				auto param_typename = il2cpp_type_get_name(method->parameters[param_idx - 1]);

				if (sig_typename.compare(param_typename)) {
					param_idx = -1; //if not set, could result in method returned with same param count but incorrect last param
					break;
				}
				if (parameters.find(' ') != std::string::npos)
					parameters.erase(0, 1);
				else
					break;
			}
			if (param_idx == method->parameters_count) {
				if (_virtual)
					return method->virtualMethodPointer;
				else
					return method->methodPointer;
			}
		}
		return 0;
	}
	void* virtual_method_from_signature(std::string signature)
	{
		return method_from_signature(signature, true);
	}

	uintptr_t Il2cppLib::field_offset_from_signature(std::string signature)
	{
		std::string klass_signature;
		size_t pos = 0;
		int arg_count = -1;

		pos = signature.find(".");
		if (pos != std::string::npos) {
			klass_signature = signature.substr(0, pos);
			signature.erase(0, pos + 1);
		}
		else {
			return 0;
		}
		auto klass = class_from_signature(klass_signature);
		if (!klass) return 0;

		return (uintptr_t)il2cpp_class_get_field_from_name(klass, signature.c_str())->offset;
	}

	namespace api
	{

		void* _new_string(std::string str)
		{
			return (void*)il2cpp_string_new_wrapper(str.c_str());
		}

		std::string get_type_mapping(std::string real)
		{
			if (type_map.find(real) != type_map.end())
				return type_map[real];
			else
				return "none";
		}
	}
}