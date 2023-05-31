#pragma once
#include <string>

#include "hooking2.hpp"

#define ORIG(name) \
static auto orig = (decltype(&name))hooking::Original((void*)name)

#define HOOK(sig) \
hooking::New( #sig , Il2cppLib::method_from_signature(hk__##sig##_sig), (void*)hk__##sig); \
hooking::Enable( #sig )

//hooking::New("pu_update", Il2cppLib::method_from_signature(hk__FP_PU_Update_sig), (void*)hk__FP_PU_Update);

//BasePlayer.Load
#include "rust/classes/BasePlayer/BasePlayer.hpp"
const static std::string hk__BP_Load_sig = "BasePlayer.Load(LoadInfo)";
void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info);

//FacePunch::PerformanceUI.update
#include "rust/classes/Facepunch/PerformanceUI/PerformanceUI.hpp"
const static std::string hk__FP_PU_Update_sig = "Facepunch::PerformanceUI.Update()";
void hk__FP_PU_Update(Facepunch::PerformanceUI* instance);

//PlayerWalkMovement.HandleJumping
#include "rust/classes/PlayerWalkMovement/PlayerWalkMovement.hpp"
const static std::string hk__PWM_HandleJumping_sig = "PlayerWalkMovement.HandleJumping(ModelState, bool, bool)";
void hk__PWM_HandleJumping(PlayerWalkMovement* instance, ModelState* state, bool wantsJump, bool jumpInDirection = false);

//HeldEntity.AddPunch
#include "rust/classes/HeldEntity/HeldEntity.hpp"
const static std::string hk__HE_AddPunch_sig = "HeldEntity.AddPunch(Vector3, float)";
void hk__HE_AddPunch(HeldEntity* instance, UnityEngine::Vector3 amount, float duration);