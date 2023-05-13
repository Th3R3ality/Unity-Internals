// Il2cppLib.cpp : Defines the functions for the static library.
//

#include "pch.h"

#include "../Include/Il2cppLib.h"
#include "api/2021.3.21f1.h"

// TODO: This is an example of a library function



namespace Il2cppLib
{
	Il2CppClass* class_from_signature(std::string signature);
	void* method_from_signature(std::string signature);
	std::string api::get_type_mapping(std::string real);

	void initialize()
	{
		api::create_type_mappings();

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

	void* method_from_signature(std::string signature)
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
			//if (method_name.compare(method->name)) continue;
			if (strcmp(method_name.c_str(), method->name)) continue;
			if (arg_count > 0 && method->parameters_count != arg_count) continue;
			
			int param_idx = 0;
			std::string parameters = signature;
			
			while (parameters.find(',') != std::string::npos) {
				param_idx += 1;
				if (param_idx > method->parameters_count) break;
				std::string sig_type = [&]() -> std::string { int pos = parameters.find(' '); auto chunk = parameters.substr(0, pos); parameters.erase(0, pos + 1); return chunk; }();
				std::string sig_name = [&]() -> std::string { int pos = parameters.find(','); auto chunk = parameters.substr(0, pos); parameters.erase(0, pos + 1); return chunk; }();

				if (api::get_type_mapping(sig_type) != "none")
					sig_type = api::get_type_mapping(sig_type);

				auto param_type = il2cpp_type_get_name(method->parameters[param_idx].parameter_type);
				auto param_name = method->parameters[param_idx].name;

				if (!sig_type.compare(param_type) || !sig_name.compare(param_name)) {
					param_idx = -1; //if not set, could result in method returned with same param count but incorrect last param
					break;
				}
				if (parameters.find(' ') != std::string::npos)
					parameters.erase(0, 1);
				else
					break;
			}
			if (param_idx == method->parameters_count)
				return method->methodPointer;
		}
		return 0;
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