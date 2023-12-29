#pragma once
#include <string>

#include "hooking2.hpp"

#define ORIG(name) \
static auto orig = (decltype(&name))hooking::Original((void*)name)

#define HOOK(sig) \
hooking::New( #sig , Il2cppLib::method_from_signature(hk__##sig##_sig), (void*)hk__##sig); \
hooking::Enable( #sig )

//hooking::New("pu_update", Il2cppLib::method_from_signature(hk__FP_PU_Update_sig), (void*)hk__FP_PU_Update);

//projectile_shoot
#include "rust/classes/BaseEntity/BaseEntity.hpp"
#include "mscorlib/System/String/String.hpp"
class MethodInfo;
const static std::string hk__projectile_shoot_sig = "projectile_shoot";
void hk__projectile_shoot(BaseEntity* instance, mscorlib::System::String* funcName, uint64_t arg1, const MethodInfo* method);

//BasePlayer.Load
#include "rust/classes/BasePlayer/BasePlayer.hpp"
const static std::string hk__BP_Load_sig = "BasePlayer.Load(LoadInfo)";
void hk__BP_Load(BasePlayer* instance, BaseNetworkable::LoadInfo info);

//BasePlayer.OnLand
#include "rust/classes/BasePlayer/BasePlayer.hpp"
const static std::string hk__BP_OnLand_sig = "BasePlayer.OnLand(BasePlayer, float)";
void hk__BP_OnLand(BasePlayer* instance, float fVelocity);

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

//PlayerVoiceSpeaker.Recieve
#include "rust/classes/PlayerVoiceSpeaker/PlayerVoiceSpeaker.hpp"
const static std::string hk__PVS_Receive_sig = "PlayerVoiceSpeaker.Receive(System.Byte[])";
void hk__PVS_Receive(PlayerVoiceSpeaker* instance, System::Array<System::Byte*>* data);

//BasePlayer.SendVoiceData
#include "rust/classes/BasePlayer/BasePlayer.hpp"
const static std::string hk__BP_SendVoiceData_sig = "BasePlayer.SendVoiceData(System.Byte[], int)";
void hk__BP_SendVoiceData(BasePlayer* instance, System::Array<System::Byte*>* data, int len);

//BasePlayer.OnVoiceData
#include "rust/classes/BasePlayer/BasePlayer.hpp"
const static std::string hk__BP_OnVoiceData_sig = "BasePlayer.OnVoiceData(System.Byte[])";
void hk__BP_OnVoiceData(BasePlayer* instance, System::Array<System::Byte*>* data);
