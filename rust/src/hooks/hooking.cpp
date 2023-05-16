#include "hooking.hpp"

#include <unordered_map>
#include <memory>

#include "minhook/include/MinHook.h"

#include "Il2cppLib.h"

namespace hooking
{

	#pragma region Hooking Class

	/*
	enum hook_state
	{
		none = 1 << 0,
		created = 1 << 1,
		enabled = 1 << 2,
		restored = 1 << 3
	};
	*/

	class hook
	{
	private:
		void* p_target;
		void* p_detour;
		void** pp_original;
		HK_STATUS state{ HK_STATUS::not_created };

	public:
		hook(void* target, void* detour) : p_target(target), p_detour(detour)
		{

		};

		~hook()
		{
			disable();
			MH_RemoveHook(p_target);
		}

		HK_STATUS setup(bool auto_enable = false)
		{
			if (state == HK_STATUS::not_created) {
				auto create_status = MH_CreateHook(p_target, p_detour, pp_original);
				if (create_status != MH_OK) { return HK_STATUS::failed_create; }
				state = HK_STATUS::created;
			}
			else return HK_STATUS::already_created;

			return state;
		}

		HK_STATUS enable()
		{
			if (state & (HK_STATUS::created | HK_STATUS::disabled)) {
				auto enable_status = MH_EnableHook(p_target);
				if (enable_status != MH_OK) { return HK_STATUS::failed_enable; }
				state = HK_STATUS::enabled;
				return state;
			}
			return state == HK_STATUS::enabled ? HK_STATUS::already_enabled : HK_STATUS::not_created;
		}

		HK_STATUS disable()
		{
			if (state == HK_STATUS::enabled) {
				auto disable_status = MH_DisableHook(p_target);
				if (disable_status != MH_OK) { return HK_STATUS::failed_disable; }
				state = HK_STATUS::disabled;
				return state;
			}
			return state == HK_STATUS::disabled ? HK_STATUS::already_disable : HK_STATUS::not_created;
		}

		HK_STATUS status() { return state; }

		HK_STATUS toggle()
		{
			if (state == HK_STATUS::enabled)
			{ return disable(); }

			if (state & (HK_STATUS::disabled | HK_STATUS::created))
			{ return enable(); }

			return state;
		}

		void** original()
		{
			return pp_original;
		}
	};

	/*
	class hookWrapperBase
	{
	public:
		virtual ~hookWrapperBase() {}
	};

	template <typename T>
	class hookWrapper : public hookWrapperBase
	{
	public:
		hookWrapper(hook<T>* hook_in) : stored_hook(hook_in) {}
		~hookWrapper() { delete stored_hook; }
		hook<T>* get() { return stored_hook; }
		int toggle() { this->get()->toggle(); }
		int status() { this->get()->status(); }
		bool enable() { this->get()->enable(); }
		bool restore() { this->get()->restore(); }

	private:
		hook<T>* stored_hook;
	};
	*/
#pragma endregion

	std::unordered_map<std::string, std::unique_ptr<hook>> _hooks;
	std::unordered_map<std::string, std::string> _il2cpp_mappings;

	HK_STATUS new_hook(std::string name, void* p_target, void* p_detour)
	{
		_hooks[name] = std::make_unique<hook>(p_target, p_detour);
		return _hooks[name].get()->setup();
	}
	HK_STATUS new_hook(std::string name, std::string il2cpp_method_signature, void* p_detour)
	{
		_il2cpp_mappings[il2cpp_method_signature] = name;
		_hooks[name] = std::make_unique<hook>(Il2cppLib::method_from_signature(il2cpp_method_signature, true), p_detour);
		return _hooks[name].get()->setup();
	}
	HK_STATUS toggle(std::string name)
	{
		return _hooks[name].get()->toggle();
	}
	HK_STATUS enable(std::string name)
	{
		return _hooks[name].get()->enable();
	}
	HK_STATUS disable(std::string name)
	{
		return _hooks[name].get()->disable();
	}
	HK_STATUS status(std::string name)
	{
		return _hooks[name].get()->status();
	}
	void restore_all()
	{
		_hooks.clear();
	}
	void** original(std::string name)
	{
		if (_il2cpp_mappings.find(name) != _il2cpp_mappings.end())
			return _hooks[_il2cpp_mappings[name]].get()->original();

		return _hooks[name].get()->original();
	}

	
	std::string get_status_message(HK_STATUS status) {
		static std::map<HK_STATUS, std::string> status_name_map;
		static bool map_initialized = false;
		if (!map_initialized) {
			status_name_map[HK_STATUS::none] = "none";
			status_name_map[HK_STATUS::not_created] = "not_created";
			status_name_map[HK_STATUS::created] = "created";
			status_name_map[HK_STATUS::enabled] = "enabled";
			status_name_map[HK_STATUS::disabled] = "disabled";
			status_name_map[HK_STATUS::failed_create] = "failed_create";
			status_name_map[HK_STATUS::failed_enable] = "failed_enable";
			status_name_map[HK_STATUS::failed_disable] = "failed_disable";
			status_name_map[HK_STATUS::already_created] = "already_created";
			status_name_map[HK_STATUS::already_enabled] = "already_enabled";
			status_name_map[HK_STATUS::already_disable] = "already_disable";
		}

		return status_name_map[status];
	}
}